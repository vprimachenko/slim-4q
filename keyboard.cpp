//
// Created by Valerij Primachenko on 20-06-20.
//

#include <numeric>
#include <vector>
#include <bitset>
#include "keyboard.h"
#include "hid.h"
#include "DAS4Q.h"
#include <optional>
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mutex>

DAS::Command::~Command() noexcept = default;

std::shared_ptr<DAS::Keyboard> DAS::findKeyboard() {
	using namespace DAS::HID4Q;

	auto path = find_keyboard_4q();
	start_keyboard(*path);

	return std::make_shared<DAS4Q>();
}


std::optional<EffectConfig> find_config(const KeyConfig& config, std::bitset<5> currentState) {
	currentState.flip();

	auto candidate = std::find_if(config.crbegin(), config.crend(), [&](const auto& a) {
		return (a.first & currentState) == 0;
	});

	if (candidate != config.crend()) {
		return candidate->second;
	} else {
		return std::nullopt;
	}
}

void DAS::Keyboard::first_update() {
	lastState[0] = GetKeyState(VK_NUMLOCK) & 0x1;
	lastState[1] = GetKeyState(VK_CAPITAL) & 0x1;
	lastState[2] = GetAsyncKeyState(VK_CONTROL) < 0;
	lastState[3] = GetAsyncKeyState(VK_MENU) < 0;
	lastState[4] = GetAsyncKeyState(VK_SHIFT) < 0;

	for (const auto&[key, config] : configs) {
		{
			auto cc = find_config(config.passive, lastState);
			if (cc.has_value()) {
				auto fx = cc.value();
				createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute();
				Sleep(1);
			}
		}
		{
			auto cc = find_config(config.active, lastState);
			if (cc.has_value()) {
				auto fx = cc.value();
				createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute();
				Sleep(1);
			}
		}
	}
	Flush()->execute();
	Sleep(1);

}

std::mutex update_lock;
void DAS::Keyboard::update() {
	if (!update_lock.try_lock()) return;

	currentState[0] = GetKeyState(VK_NUMLOCK) & 0x1;
	currentState[1] = GetKeyState(VK_CAPITAL) & 0x1;
	currentState[2] = GetAsyncKeyState(VK_CONTROL) < 0;
	currentState[3] = GetAsyncKeyState(VK_MENU) < 0;
	currentState[4] = GetAsyncKeyState(VK_SHIFT) < 0;

	if (currentState != lastState) {
		for (const auto&[key, config] : configs) {
			{
				auto cc = find_config(config.active, currentState);
				auto lastcc = find_config(config.active, lastState);

				if (cc.has_value() && cc != lastcc) {
					auto fx = cc.value();
					createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute();
					Sleep(1);
				}
			}
			{
				if (overrides.contains(key)) continue;

				auto cc = find_config(config.passive, currentState);
				auto lastcc = find_config(config.passive, lastState);

				if (cc.has_value() && cc != lastcc) {
					auto fx = cc.value();
					createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute();
					Sleep(1);
				}
			}
		}
		Flush()->execute();
		Sleep(1);
	}

	lastState = currentState;
	update_lock.unlock();
}

void DAS::Keyboard::read_config(const nlohmann::json &j) {
	configs = parse_config(j);
	first_update();
}

bool DAS::Keyboard::add_override(const std::string& pid, const Key &key, const EffectConfig &fx) {
	if (createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute()) {
		Sleep(1);
		Flush()->execute();
		Sleep(1);
		overrides[key] = pid;
		return true;
	}
	return false;
}

bool DAS::Keyboard::remove_override(const std::string& pid) {
	auto needle = std::find_if(overrides.begin(), overrides.end(), [&](auto pair) {
		return pair.second == pid;
	});

	if (needle == overrides.end()) { return false; }

	const auto key = needle->first;

	if (configs.contains(needle->first)) {
		auto lastcc = find_config(configs[key].passive, lastState);

		if(lastcc.has_value()) {
			auto fx = lastcc.value();
			createEffect(fx.fx, key, fx.r, fx.g, fx.b)->execute();
		} else {
			Wipe(key)->execute();
		}
		Sleep(1);
	} else {
		Wipe(key)->execute();
	}
	Flush()->execute();
	Sleep(1);

	overrides.erase(needle);

	return true;
}
