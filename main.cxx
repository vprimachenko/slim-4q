#include <windows.h>
#include <algorithm>


#include "constants.h"
#include "keyboard.h"


static const auto letter_keys = {
	Keys::Q, Keys::W, Keys::E, Keys::R, Keys::T, Keys::Y, Keys::U, Keys::I, Keys::O, Keys::P, Keys::BracketOpen, Keys::BracketClose,
	Keys::A, Keys::S, Keys::D, Keys::F, Keys::G, Keys::H, Keys::J, Keys::K, Keys::L, Keys::Semicolon, Keys::Quote,
	Keys::Z, Keys::X, Keys::C, Keys::V, Keys::B, Keys::N, Keys::M, Keys::Comma, Keys::Dot, Keys::Slash
};

static const auto numpad_keys = {
	Keys:: Num0, Keys::Num1, Keys::Num2, Keys::Num3, Keys::Num4, Keys::Num5, Keys::Num6, Keys::Num7, Keys::Num8, Keys::Num9
};

static const auto ctrl_keys = {
	Keys::A, Keys::S, Keys::C, Keys::X, Keys::V
};

static const auto alt_keys = {
	Keys::F4, Keys::J
};

int main(int argc, char const *argv[]) {
	using namespace DAS;

	auto keyboard = DAS::findKeyboard();

	std::vector<std::shared_ptr<DAS::Command>> commands;

	commands.reserve(letter_keys.size());

	std::transform(letter_keys.begin(), letter_keys.end(), std::back_inserter(commands), [&](Keys a) {
		return keyboard->Passive_Color(a, 0, 0, 250);
	});

	commands.emplace_back(keyboard->Active_Ripple(Keys::CapsLock, 0, 0, 250));

	for (auto command: commands) {
		keyboard->Wipe(command->key)->execute();
		Sleep(1);
		command->execute();
	}

	keyboard->Flush()->execute();
	Sleep(1);


	std::vector<std::shared_ptr<DAS::Command>> num_commands;

	num_commands.reserve(numpad_keys.size());

	std::transform(numpad_keys.begin(), numpad_keys.end(), std::back_inserter(num_commands), [&](Keys a) {
		return keyboard->Passive_Color(a, 0, 0, 250);
	});


	std::vector<std::shared_ptr<DAS::Command>> ctrl_commands;

	ctrl_commands.reserve(2 * ctrl_keys.size());

	std::transform(ctrl_keys.begin(), ctrl_keys.end(), std::back_inserter(ctrl_commands), [&](Keys a) {
		return keyboard->Passive_Color(a, 250, 250, 2);
	});
	std::transform(ctrl_keys.begin(), ctrl_keys.end(), std::back_inserter(ctrl_commands), [&](Keys a) {
		return keyboard->Active_Ripple(a, 250, 250, 2);
	});


	std::vector<std::shared_ptr<DAS::Command>> alt_commands;

	alt_commands.reserve(2 * alt_keys.size());

	std::transform(alt_keys.begin(), alt_keys.end(), std::back_inserter(alt_commands), [&](Keys a) {
		return keyboard->Passive_Color(a, 250, 250, 2);
	});
	std::transform(alt_keys.begin(), alt_keys.end(), std::back_inserter(alt_commands), [&](Keys a) {
		return keyboard->Active_Ripple(a, 250, 250, 2);
	});


	while (true) {
		{
			static auto old_numlock = 0;
			auto numlock = GetKeyState(VK_NUMLOCK) & 0x1;

			if (numlock != old_numlock) {
				old_numlock = numlock;
				for (auto command: num_commands) {
					if (numlock) {
						command->execute();
					} else {
						auto def = std::find_if(commands.begin(), commands.end(), [&] (auto a) {
							return a->key == command->key;
						});

						if (def != commands.end()) {
							(*def)->execute();
						} else {
							keyboard->Passive_Color(command->key, 0, 0, 0)->execute();
						}
					}
					Sleep(1);
				}
				keyboard->Flush()->execute();
				Sleep(1);
			}
		}

		{
			static auto old_ctrl = false;
			auto ctrl = GetAsyncKeyState(VK_CONTROL) < 0;

			if (ctrl != old_ctrl) {
				old_ctrl = ctrl;
				for (auto command: alt_commands) {
					if (ctrl) {
						command->execute();
					} else {
						auto def = std::find_if(commands.begin(), commands.end(), [&] (auto a) {
							return a->key == command->key;
						});

						if (def != commands.end()) {
							(*def)->execute();
						} else {
							keyboard->Passive_Color(command->key, 0, 0, 0)->execute();
						}
						Sleep(1);
						keyboard->Active_Color(command->key, 0, 250, 20, 1)->execute();
					}
					Sleep(1);
				}
				keyboard->Flush()->execute();
				Sleep(1);
			}
		}

		{
			static auto old_alt = false;
			auto alt = GetAsyncKeyState(VK_MENU) < 0;

			if (alt != old_alt) {
				old_alt = alt;
				for (auto command: ctrl_commands) {
					if (alt) {
						command->execute();
					} else {
						auto def = std::find_if(commands.begin(), commands.end(), [&] (auto a) {
							return a->key == command->key;
						});

						if (def != commands.end()) {
							(*def)->execute();
						} else {
							keyboard->Passive_Color(command->key, 0, 0, 0)->execute();
						}
						Sleep(1);
						keyboard->Active_Color(command->key, 0, 250, 20, 1)->execute();
					}
					Sleep(1);
				}
				keyboard->Flush()->execute();
				HID4Q::get_report();
			}
		}

		Sleep(20);
	}

	return 0;
}