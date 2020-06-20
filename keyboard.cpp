//
// Created by le0n on 20-06-20.
//

#include <numeric>
#include "keyboard.h"

DAS::Command::~Command() noexcept {}

std::shared_ptr<DAS::Keyboard> DAS::findKeyboard() {
	using namespace DAS::HID4Q;

	auto path = find_keyboard_4q();
	start_keyboard(*path.get());

	return std::make_shared<DAS4Q>();
}

constexpr std::size_t ceil_to_multiple(std::size_t n, std::size_t mul) {
	return (n + mul - 1) / mul * mul;
};

void DAS::DAS4Q::Command4Q::execute() {
	std::vector<uint8_t> ret(ceil_to_multiple(2 + buffer.size() + 1, 7), 0);
	ret[0] = 0xEA;
	ret[1] = buffer.size() + 1;
	std::copy(buffer.begin(), buffer.end(), ret.begin() + 2);

	auto x_o_r = [](auto a, auto b) {
		return a ^ b;
	};

	auto checksum = std::accumulate(buffer.begin(), buffer.end(), 0xEA ^ ret[1], x_o_r);

	ret[buffer.size() + 2] = checksum;

	DAS::HID4Q::set_report(ret);
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Flush() {
	return std::make_shared<DAS4Q::Command4Q>(Keys::Dummy, std::vector<uint8_t>{0x78, 0x0A});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Wipe(Keys key) {
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x03, static_cast<uint8_t>(key), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});;
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Passive_Color(Keys key, uint8_t r, uint8_t g, uint8_t b) {
	static const uint8_t fx = 0x01;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x08, static_cast<uint8_t>(key), fx, r, g, b});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Active_Color(Keys key, uint8_t r, uint8_t g, uint8_t b, uint8_t delay) {
	static const uint8_t fx = 0x1E;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x04, static_cast<uint8_t>(key), fx, r, g, b, delay, 0x00, 0x00});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Passive_Blink(Keys key, uint8_t r, uint8_t g, uint8_t b) {
	static const uint8_t fx = 0x1f;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x08, static_cast<uint8_t>(key), fx, r, g, b, 0, 0, 0});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Blink_(Keys key, uint8_t r, uint8_t g, uint8_t b) {
	static const uint8_t fx = 0x1f;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x04, static_cast<uint8_t>(key), fx, r, g, b, 0, 0, 0});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Active_Ripple(Keys key, uint8_t r, uint8_t g, uint8_t b) {
	static const uint8_t fx = 0x11;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x04, static_cast<uint8_t>(key), fx, r, g, b, 0, 0, 0});
}

std::shared_ptr<DAS::Command> DAS::DAS4Q::Active_InverseRipple(Keys key, uint8_t r, uint8_t g, uint8_t b) {
	static const uint8_t fx = 0x21;
	return std::make_shared<DAS4Q::Command4Q>(key, std::vector<uint8_t>{0x78, 0x04, static_cast<uint8_t>(key), fx, r, g, b, 0, 0, 0});
}

DAS::DAS4Q::~DAS4Q() {
	DAS::HID4Q::stop_keyboard();
}
