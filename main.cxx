#include <fstream>

#include "json/json.hpp"
#include "keyboard.h"
#include "rest_api.h"
#include "plugins.h"
#include "trayicon.h"

std::shared_ptr<DAS::Keyboard> keyboard;

int main(int argc, char const *argv[]) {
	keyboard = DAS::findKeyboard();

	std::ifstream i("config.json");
	nlohmann::json j;
	i >> j;

	keyboard->read_config(j);

	start_server(keyboard);
	start_python(keyboard);

	show_trayicon(keyboard);

	stop_server();
	stop_python();

	return 0;
}

