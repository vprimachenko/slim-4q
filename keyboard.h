//
// Created by le0n on 20-06-20.
//

#ifndef DAS_KEYBOARD_H
#define DAS_KEYBOARD_H

#include <cstdint>
#include <memory>
#include "constants.h"
#include "hid.h"

namespace DAS {
	struct Command {
		Keys key;
		std::vector<uint8_t> buffer;

		Command(Keys key, std::vector<uint8_t> buffer)
				: key(key), buffer(buffer) {}

		virtual void execute() = 0;

		virtual ~Command() = 0;
	};

	struct Keyboard {
		virtual std::shared_ptr<Command> Flush() = 0;

		virtual std::shared_ptr<Command> Wipe(Keys key) = 0;

		virtual std::shared_ptr<Command> Passive_Color(Keys key, uint8_t r, uint8_t g, uint8_t b) = 0;

		virtual std::shared_ptr<Command> Passive_Blink(Keys key, uint8_t r, uint8_t g, uint8_t b) = 0;

		virtual std::shared_ptr<Command> Blink_(Keys key, uint8_t r, uint8_t g, uint8_t b) = 0;

		virtual std::shared_ptr<Command> Active_Ripple(Keys key, uint8_t r, uint8_t g, uint8_t b) = 0;

		virtual std::shared_ptr<Command> Active_InverseRipple(Keys key, uint8_t r, uint8_t g, uint8_t b) = 0;

		virtual std::shared_ptr<Command> Active_Color(Keys key, uint8_t r, uint8_t g, uint8_t b, uint8_t delay) = 0;
	};

	std::shared_ptr<Keyboard> findKeyboard();

	struct DAS4Q : public Keyboard {

		std::shared_ptr<Command> Flush() override;

		std::shared_ptr<Command> Wipe(Keys key) override;

		std::shared_ptr<Command> Passive_Color(Keys key, uint8_t r, uint8_t g, uint8_t b) override;

		std::shared_ptr<Command> Passive_Blink(Keys key, uint8_t r, uint8_t g, uint8_t b) override;

		std::shared_ptr<Command> Blink_(Keys key, uint8_t r, uint8_t g, uint8_t b) override;

		std::shared_ptr<Command> Active_Ripple(Keys key, uint8_t r, uint8_t g, uint8_t b) override;

		std::shared_ptr<Command> Active_InverseRipple(Keys key, uint8_t r, uint8_t g, uint8_t b) override;

		std::shared_ptr<Command> Active_Color(Keys key, uint8_t r, uint8_t g, uint8_t b, uint8_t delay) override;

		struct Command4Q: public DAS::Command {
			Command4Q(Keys key, std::vector<uint8_t> buffer)
				: Command(key, buffer) {}

			void execute();
		};

		DAS4Q() {}

		~DAS4Q();
	};
}

#endif //DAS_KEYBOARD_H
