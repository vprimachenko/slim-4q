//
// Created by Valerij Primachenko on 20-06-18.
//
#define LOG

#ifdef __unix__                     /* __unix__ is usually defined by compilers targeting Unix systems */

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#include <windows.h>
#include <setupapi.h>
#include <hidusage.h>
#include <hidpi.h>
#include <hidsdi.h>

#include <vector>
#include <cassert>



#include "hid.h"

#ifdef LOG
#include <iostream>
#include <iomanip>
template<class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &input) {
	std::ios_base::fmtflags f(os.flags());
	for (auto const &i: input) {
		os << " " << std::hex << std::setw(2) << std::setfill('0') << (int) i;
	}
	os.flags(f);
	return os;
}
#endif


namespace DAS {
	namespace HID4Q {
		HANDLE	handle;
		PHIDP_PREPARSED_DATA    _preparsedData;

		static const uint8_t reportID = 0x01;
		const static std::vector<uint8_t> OK_RESPONSE{0xed, 0x03, 0x78, 0x00, 0x96, 0x00, 0x00, 0x00};
		const static std::vector<uint8_t> NOT_READY{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		std::shared_ptr<char *> find_keyboard_4q() {
			char *_tpath = NULL; // "\\\\?\\hid#vid_24f0&pid_2037&mi_01&col01#7&189cac3&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}"

			GUID    guid;
			HidD_GetHidGuid(&guid);

			// Get a pointer to the device information set
			HANDLE info = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
			if( INVALID_HANDLE_VALUE == info )
				return std::make_shared<char *>(nullptr);

			void* buffer = NULL;
			for (unsigned i = 0; true; ++i) {
				SP_DEVICE_INTERFACE_DATA devInterface = { .cbSize = sizeof(SP_DEVICE_INTERFACE_DATA) };
				if( !SetupDiEnumDeviceInterfaces(info, NULL, &guid, i, &devInterface) )
					break;


				// Get the required buffer size for the interface's details
				DWORD size;
				if( !SetupDiGetDeviceInterfaceDetail(info, &devInterface, NULL, 0, &size, NULL) && (122 != GetLastError()))
					break;

				buffer = realloc(buffer, size);

				auto detail = new(buffer) SP_DEVICE_INTERFACE_DETAIL_DATA {.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA)};

				if( !SetupDiGetDeviceInterfaceDetail(info, &devInterface, detail, size, NULL, NULL) )
					break;

				static const char needle[] = "\\\\?\\hid#vid_24f0&pid_2037&mi_01&col01#";
				auto match = strncmp(detail->DevicePath, needle, strlen(needle)) == 0;
				if (match) {
					_tpath = strdup(detail->DevicePath);
					printf("FOUND DAS 4Q at %s\n", detail->DevicePath);
				}
			}
			free(buffer);
			SetupDiDestroyDeviceInfoList(info);

			return std::make_shared<char *>(_tpath);
		}

		void start_keyboard(char *_tpath) {
			handle = CreateFile(_tpath,  GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			HidD_GetPreparsedData(handle, &_preparsedData);
		}

		void set_report(const std::vector<uint8_t> &command) {
#ifdef LOG
			std::cout << "SET_REPORT  ";
#endif
			for (auto i = command.begin(); i != command.end(); i += 7) {
				std::vector<uint8_t> buffer(i, i + 7);

				uint8_t b[8];
				auto length = 8;

				HidP_InitializeReportForID(HidP_Feature, reportID, _preparsedData, (char*)b, length);
				std::copy(buffer.begin(), buffer.end(), b+1);
				HidD_SetFeature(handle, b, length);
#ifdef LOG
				std::cout << buffer << " " ;
#endif
			}
#ifdef LOG
			std::cout << std::endl;
#endif
		}

		std::vector<uint8_t> get_report() {
			uint8_t b[8];
			auto length = 8;

			std::vector<uint8_t> buffer;

			HidP_InitializeReportForID(HidP_Feature, reportID, _preparsedData, (char*)b, length);
			if( HidD_GetFeature(handle, b, length) ) {
				buffer = {b, b+length};
			} else {
				buffer = {};
			}
#ifdef LOG
			std::cout << "GET_REPORT  " << buffer;
			if (buffer == OK_RESPONSE) { std::cout << " ok"; }
			else if (buffer == NOT_READY) {
				std::cout << " not ready";
			} else { std::cout << " ERROR"; }
			std::cout << std::endl;
#endif
			return std::move(buffer);
		}

		void stop_keyboard() {
			CloseHandle(handle);
		}
	}
}

#endif
