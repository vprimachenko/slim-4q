//
// Created by Valerij Primachenko on 20-06-20.
//

#ifndef DAS_KEYBOARD_H
#define DAS_KEYBOARD_H

#include <cstdint>
#include <bitset>
#include <memory>
#include <map>
#include <set>
#include "keys.h"
#include "json/json.hpp"
#include "effect.h"
#include "Config.h"

namespace DAS {
	struct Command {
		Key key;

		Command(Key key)
				: key(key) {}

		virtual bool execute() = 0;

		virtual ~Command() = 0;
	};

	class Keyboard {
		std::map<Key, Config> configs;
		std::map<Key, std::string> overrides;

		virtual std::shared_ptr<Command> Flush() = 0;
		virtual std::shared_ptr<Command> Wipe(Key key) = 0;
		virtual std::shared_ptr<Command> createEffect(Effect effect, Key key, uint8_t r, uint8_t g, uint8_t b) = 0;

		std::bitset<5> currentState, lastState;
		void first_update();

	public:
		void read_config(const nlohmann::json &j);
		void update();
		bool add_override(const std::string& pid, const Key& key, const EffectConfig& fx);
		bool remove_override(const std::string& pid);
	};

	std::shared_ptr<Keyboard> findKeyboard();
}

#endif //DAS_KEYBOARD_H
