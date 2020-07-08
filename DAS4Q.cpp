//
// Created by Valerij Primachenko on 20-07-01.
//

#include "DAS4Q.h"
#include "hid.h"

std::shared_ptr<DAS::Command> DAS::DAS4Q::createEffect(DAS::Effect effect, Key key, uint8_t r, uint8_t g, uint8_t b) {
	return std::make_shared<DAS::DAS4Q::EffectCommand>(effect, key, r, g, b);
}

uint8_t DAS::DAS4Q::codeFromKey(Key key) {
	switch (key) {
		case Key::KEY_CONTROL_LEFT: return 0;
		case Key::KEY_SHIFT_LEFT: return 1;
		case Key::KEY_CAPS_LOCK: return 2;
		case Key::KEY_TAB: return 3;
		case Key::KEY_BACKTICK: return 4;
		case Key::KEY_ESCAPE: return 5;

		case Key::KEY_META_LEFT: return 6;
		case Key::KEY_A: return 8;
		case Key::KEY_Q: return 9;
		case Key::KEY_1: return 10;

		case Key::KEY_ALT_LEFT: return 12;
		case Key::KEY_Z: return 13;
		case Key::KEY_S: return 14;
		case Key::KEY_W: return 15;
		case Key::KEY_2: return 16;
		case Key::KEY_F1: return 17;

		case Key::KEY_X: return 19;
		case Key::KEY_D: return 20;
		case Key::KEY_E: return 21;
		case Key::KEY_3: return 22;
		case Key::KEY_F2: return 23;

		case Key::KEY_C: return 25;
		case Key::KEY_F: return 26;
		case Key::KEY_R: return 27;
		case Key::KEY_4: return 28;
		case Key::KEY_F3: return 29;

		case Key::KEY_V: return 31;
		case Key::KEY_G: return 32;
		case Key::KEY_T: return 33;
		case Key::KEY_5: return 34;
		case Key::KEY_F4: return 35;

		case Key::KEY_SPACE: return 36;
		case Key::KEY_B: return 37;
		case Key::KEY_H: return 38;
		case Key::KEY_Y: return 39;
		case Key::KEY_6: return 40;
		case Key::KEY_F5: return 41;

		case Key::KEY_N: return 43;
		case Key::KEY_J: return 44;
		case Key::KEY_U: return 45;
		case Key::KEY_7: return 46;
		case Key::KEY_F6: return 47;

		case Key::KEY_M: return 49;
		case Key::KEY_K: return 50;
		case Key::KEY_I: return 51;
		case Key::KEY_8: return 52;
		case Key::KEY_F7: return 53;

		case Key::KEY_COMMA: return 55;
		case Key::KEY_L: return 56;
		case Key::KEY_O: return 57;
		case Key::KEY_9: return 58;
		case Key::KEY_F8: return 59;

		case Key::KEY_ALT_RIGHT: return 60;
		case Key::KEY_DOT: return 61;
		case Key::KEY_SEMICOLON: return 62;
		case Key::KEY_P: return 63;
		case Key::KEY_0: return 64;
		case Key::KEY_F9: return 65;

		case Key::KEY_META_RIGHT: return 66;
		case Key::KEY_SLASH: return 67;
		case Key::KEY_QUOTE: return 68;
		case Key::KEY_OPEN_SQUARE_BRACKET: return 69;
		case Key::KEY_SUBTRACT: return 70;
		case Key::KEY_F10: return 71;

		case Key::KEY_CONTEXT_MENU: return 72;
		case Key::KEY_CLOSE_SQUARE_BRACKET: return 75;
		case Key::KEY_EQUAL: return 76;
		case Key::KEY_F11: return 77;

		case Key::KEY_CONTROL_RIGHT: return 78;
		case Key::KEY_SHIFT_RIGHT: return 79;
		case Key::KEY_ENTER: return 80;
		case Key::KEY_BACKSLASH: return 81;
		case Key::KEY_BACKSPACE: return 82;
		case Key::KEY_F12: return 83;

		case Key::KEY_ARROW_LEFT: return 84;
		case Key::KEY_DELETE: return 87;
		case Key::KEY_INSERT: return 88;
		case Key::KEY_PRINT_SCREEN: return 89;

		case Key::KEY_ARROW_DOWN: return 90;
		case Key::KEY_ARROW_UP: return 91;
		case Key::KEY_END: return 93;
		case Key::KEY_HOME: return 94;
		case Key::KEY_SCROLL_LOCK: return 95;

		case Key::KEY_ARROW_RIGHT: return 96;
		case Key::KEY_PAGE_DOWN: return 99;
		case Key::KEY_PAGE_UP: return 100;
		case Key::KEY_PAUSE_BREAK: return 101;

		case Key::KEY_NUMPAD_0: return 102;
		case Key::KEY_NUMPAD_1: return 103;
		case Key::KEY_NUMPAD_4: return 104;
		case Key::KEY_NUMPAD_7: return 105;
		case Key::KEY_NUMLOCK: return 106;

		case Key::KEY_NUMPAD_2: return 109;
		case Key::KEY_NUMPAD_5: return 110;
		case Key::KEY_NUMPAD_8: return 111;
		case Key::KEY_NUMPAD_DIVIDE: return 112;

		case Key::KEY_NUMPAD_DECIMAL: return 114;
		case Key::KEY_NUMPAD_3: return 115;
		case Key::KEY_NUMPAD_6: return 116;
		case Key::KEY_NUMPAD_9: return 117;
		case Key::KEY_NUMPAD_MULTIPLY: return 118;

		case Key::KEY_NUMPAD_ENTER: return 122;
		case Key::KEY_NUMPAD_ADD: return 123;
		case Key::KEY_NUMPAD_SUBTRACT: return 124;

		case Key::KEY_SLEEP: return 126;
		case Key::KEY_BRIGHTNESS_KEYBOARD: return 127;
		case Key::KEY_PLAY: return 128;
		case Key::KEY_NEXT: return 129;

		case Key::KEY_VOLUME_Q_BUTTON: return 130;
		case Key::NOT_IN_PHYSICAL_DEVICE: return 250;
	}
}


constexpr std::size_t ceil_to_multiple(std::size_t n, std::size_t mul) {
	return (n + mul - 1) / mul * mul;
};


static constexpr uint8_t PASSIVE_COMMAND = 0x08;
static constexpr uint8_t ACTIVE_COMMAND = 0x04;
static constexpr uint8_t WIPE_COMMAND = 0x03;

bool DAS::DAS4Q::EffectCommand::execute() {
	std::vector<uint8_t> buffer;

	switch (effect) {

		case Effect::PASSIVE_NONE:
			buffer = {0x78, PASSIVE_COMMAND, codeFromKey(key), 0x01, 0x00, 0x00, 0x00};
			break;

		case Effect::PASSIVE_SET:
			buffer = {0x78, PASSIVE_COMMAND, codeFromKey(key), 0x01, r, g, b};
			break;

		case Effect::PASSIVE_BLINK:
			buffer = {0x78, PASSIVE_COMMAND, codeFromKey(key), 0x1f, r, g, b};
			break;

		case Effect::PASSIVE_BREATHE:
			buffer = {0x78, PASSIVE_COMMAND, codeFromKey(key), 0x08, r, g, b};

		case Effect::PASSIVE_COLOR_CYCLE:
			buffer = {0x78, PASSIVE_COMMAND, codeFromKey(key), 0x14, r, g, b};
			break;

		case Effect::ACTIVE_NONE:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
			break;

		case Effect::ACTIVE_SET:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x1e, r, g, b, 0x07, 0xd0, 0x00};
			break;

		case Effect::ACTIVE_BLINK:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x1f, r, g, b, 0x01, 0xf4, 0x03};
			break;

		case Effect::ACTIVE_BREATHE:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x08, r, g, b, 0x03, 0xe8, 0x03};
			break;

		case Effect::ACTIVE_COLOR_CYCLE:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x14, r, g, b, 0x13, 0x88, 0x00};
			break;

		case Effect::ACTIVE_RIPPLE:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x11, r, g, b, 0, 0, 0};
			break;

		case Effect::ACTIVE_INV_RIPPLE:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x21, r, g, b, 0, 0, 0};
			break;

		case Effect::ACTIVE_LASER:
			buffer = {0x78, ACTIVE_COMMAND, codeFromKey(key), 0x0b, r, g, b, 0, 0, 0};
			break;

		default: return false;
	}


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

	return true;
}

//-------------------------------------------------------------------------

std::shared_ptr<DAS::Command> DAS::DAS4Q::Flush() {
	return std::make_shared<DAS4Q::FlushCommand>();
}

bool DAS::DAS4Q::FlushCommand::execute() {
	std::vector<uint8_t> buffer {0x78, 0x0A};
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

	return DAS::HID4Q::get_report() == DAS::HID4Q::OK_RESPONSE;
}

//-------------------------------------------------------------------------

std::shared_ptr<DAS::Command> DAS::DAS4Q::Wipe(DAS::Key key) {
	return std::make_shared<DAS4Q::WipeCommand>(key);
}

bool DAS::DAS4Q::WipeCommand::execute() {
	std::vector<uint8_t> buffer {0x78, WIPE_COMMAND, codeFromKey(key), 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
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

	return true;
}

//-------------------------------------------------------------------------

DAS::DAS4Q::~DAS4Q() {
	DAS::HID4Q::stop_keyboard();
}
