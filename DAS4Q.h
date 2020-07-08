//
// Created by Valerij Primachenko on 20-07-01.
//

#ifndef DAS_DAS4Q_H
#define DAS_DAS4Q_H

#include <memory>
#include "keys.h"
#include "keyboard.h"

namespace DAS {
	struct DAS4Q : public Keyboard {
		static uint8_t codeFromKey(Key key);

		std::shared_ptr <Command> Flush() override;

		std::shared_ptr <Command> Wipe(Key key) override;

		std::shared_ptr <Command> createEffect(Effect effect, Key key, uint8_t r, uint8_t g, uint8_t b) override;

		struct FlushCommand : public DAS::Command {
			FlushCommand()
					: Command(Key::NOT_IN_PHYSICAL_DEVICE) {}

			bool execute() override;
		};

		struct WipeCommand : public DAS::Command {
			WipeCommand(Key key)
					: Command(key) {}

			bool execute() override;
		};

		struct EffectCommand : public DAS::Command {
			Effect effect;

			uint8_t r, g, b;

			EffectCommand(Effect effect, Key key, uint8_t r, uint8_t g, uint8_t b)
					: Command(key), effect(effect), r(r), g(g), b(b) {}

			bool execute() override;
		};

		DAS4Q() {}

		~DAS4Q();
	};
}

#endif //DAS_DAS4Q_H
