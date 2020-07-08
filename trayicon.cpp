//
// Created by Valerij Primachenko on 20-06-21.
//

#define UNICODE

#include "resource.h"
#include "keyboard.h"
#include <windows.h>
#include <shellapi.h>
#include <strsafe.h>
#include <memory>

std::shared_ptr<DAS::Keyboard> tray_keyboard;

HINSTANCE g_hInst = NULL;

UINT_PTR const HIDEFLYOUT_TIMER_ID = 1;
UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;

wchar_t const szWindowClass[] = L"DERCUSTOMIZER";

// Use a guid to uniquely identify our icon
class __declspec(uuid("d5d40987-a38d-4dee-976b-a32c7aa82b2d")) PrinterIcon;

// Forward declarations of functions included in this code module:
void                RegisterWindowClass(PCWSTR pszClassName, PCWSTR pszMenuName, WNDPROC lpfnWndProc);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                ShowContextMenu(HWND hwnd, POINT pt);
BOOL                AddNotificationIcon(HWND hwnd);
BOOL                DeleteNotificationIcon();

void show_trayicon(std::shared_ptr<DAS::Keyboard> keyboard) {
	tray_keyboard = keyboard;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	int nCmdShow =  SW_HIDE;
	g_hInst = hInstance;
	RegisterWindowClass(szWindowClass, MAKEINTRESOURCE(IDC_NOTIFICATIONICON), WndProc);

	HWND hwnd = CreateWindow(szWindowClass, szWindowClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 250, 200, NULL, NULL, g_hInst, NULL);

	if (hwnd) {
		ShowWindow(hwnd, nCmdShow);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void RegisterWindowClass(PCWSTR pszClassName, PCWSTR pszMenuName, WNDPROC lpfnWndProc) {
	WNDCLASSEX wcex = {
		.cbSize         = sizeof(WNDCLASSEX),
		.style          = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc    = lpfnWndProc,
		.hInstance      = g_hInst,
		.hIcon          = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_NOTIFICATIONICON)),
		.hCursor        = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1),
		.lpszMenuName   = pszMenuName,
		.lpszClassName  = pszClassName
	};

	RegisterClassEx(&wcex);
}

BOOL AddNotificationIcon(HWND hwnd) {
	NOTIFYICONDATA nid = {
		.cbSize           = sizeof(NOTIFYICONDATA),
		.hWnd             = hwnd,
		.uFlags           = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP | NIF_GUID,
		.uCallbackMessage = WMAPP_NOTIFYCALLBACK,
		.hIcon            = LoadIcon(g_hInst, MAKEINTRESOURCE(TRAY_ICON)),
		.szTip            = reinterpret_cast<WCHAR>(szWindowClass), //todo fix
		.guidItem         = __uuidof(PrinterIcon)
	};

	Shell_NotifyIcon(NIM_ADD, &nid);

	nid.uVersion = NOTIFYICON_VERSION_4;
	return Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

BOOL DeleteNotificationIcon() {
	NOTIFYICONDATA nid = {
		.cbSize   = sizeof(NOTIFYICONDATA),
		.uFlags   = NIF_GUID,
		.guidItem = __uuidof(PrinterIcon)
	};

	return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowContextMenu(HWND hwnd, POINT pt) {
	HMENU hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDC_CONTEXTMENU));
	if (hMenu) 	{
		HMENU hSubMenu = GetSubMenu(hMenu, 0);
		if (hSubMenu) {
			SetForegroundWindow(hwnd);

			UINT uFlags = TPM_RIGHTBUTTON;
			if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
				uFlags |= TPM_RIGHTALIGN;
			} else {
				uFlags |= TPM_LEFTALIGN;
			}

			TrackPopupMenuEx(hSubMenu, uFlags, pt.x, pt.y, hwnd, NULL);
		}
		DestroyMenu(hMenu);
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_CREATE:
			// add the notification icon
			if (!AddNotificationIcon(hwnd)) {
				MessageBox(hwnd, L"cant add tray icon for driver control", L"Error adding icon", MB_OK);
				return -1;
			}

			SetTimer(hwnd, HIDEFLYOUT_TIMER_ID, 50, (TIMERPROC) NULL);
			break;

		case WM_COMMAND: {
			int const wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_OPTIONS:
					ShellExecute(0, 0, L"http://localhost:27301/", 0, 0 , SW_SHOW );
					break;

				case IDM_EXIT:
					DestroyWindow(hwnd);
					break;

				default:
					return DefWindowProc(hwnd, message, wParam, lParam);
			}
		} break;

		case WMAPP_NOTIFYCALLBACK:
			switch (LOWORD(lParam)) {
				case WM_CONTEXTMENU:
				{
					POINT const pt = { LOWORD(wParam), HIWORD(wParam) };
					ShowContextMenu(hwnd, pt);
				}
					break;
			}
			break;


		case WM_TIMER:
			if (wParam == HIDEFLYOUT_TIMER_ID) {
				tray_keyboard->update();
			}
			break;

		case WM_DESTROY:
			DeleteNotificationIcon();
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
