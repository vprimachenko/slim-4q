//
// Created by Valerij Primachenko on 20-06-18.
//

#ifndef DAS_HID_H
#define DAS_HID_H

#include <memory>

namespace DAS {
	namespace HID4Q {

		extern const std::vector<uint8_t> OK_RESPONSE;
		extern const std::vector<uint8_t> NOT_READY;

		std::shared_ptr<char *> find_keyboard_4q();

		void start_keyboard(char *_tpath);

		void set_report(const std::vector<uint8_t> &);

		std::vector<uint8_t> get_report();

		void stop_keyboard();
	}
}
#endif //DAS_HID_H
