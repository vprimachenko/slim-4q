//
// Created by Valerij Primachenko on 20-06-19.
//
#include <string>
#include <fstream>
#include <streambuf>

#include "rest_api.h"


#include "webserver/httplib.h"
#include "json/json.hpp"

#include "resource.h"
#include "keys.h"
#include "deps/magic_enum.hpp"
#include "keyboard.h"

std::shared_ptr<httplib::Server> server;
std::thread server_thread;
std::shared_ptr<DAS::Keyboard> srv_keyboard;


std::shared_ptr<httplib::Server> run_server() {
	using namespace httplib;

	auto svr = std::make_shared<Server>();

//	HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(INDEX_HTML), RT_RCDATA);
//	unsigned int myResourceSize = ::SizeofResource(NULL, myResource);
//	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
//	void* pMyBinaryData = ::LockResource(myResourceData);

	svr->Get("/favicon.ico", [](const Request& req, Response& res) {
	});

	//*
	svr->set_mount_point("/", "./../customizer/dist");
	/*/
	svr->Get("/", [](const Request& req, Response& res) {
		res.set_content(static_cast<char*>(pMyBinaryData), myResourceSize, "text/html; charset=utf-8");
	});
	/**/

	svr->Post("/api/1.0/signals", [&](const Request& req, Response& res) {
		auto payload = nlohmann::json::parse(req.body); //.get<Payload>();

//		printf("request to set %d to %s %s\n" , payload.key, payload.effect.c_str(), payload.color.c_str());

		auto zone_id = magic_enum::enum_cast<DAS::Key>(payload.at("zoneId").get<std::string>());
		DAS::Effect effect = DAS::Effect::ACTIVE_SET;
		if (payload.contains("effect")) {
			auto _effect = magic_enum::enum_cast<DAS::Effect>(payload.at("effect").get<std::string>());
			if (_effect.has_value()) {
				effect = _effect.value();
			}
		}
		auto pid = payload.at("effect").get<std::string>();

		int r, g, b;
		auto color_string = payload.at("color").get<std::string>();
		auto scan_res = sscanf(color_string.c_str(), "#%02x%02x%02x", &r, &g, &b);

		if (zone_id.has_value() && scan_res == 3) {
			bool ret = srv_keyboard->add_override(pid, zone_id.value(), {effect, static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)});

			res.set_content(ret ? "true" : "false", "application/json; charset=utf-8");
		} else {
			res.set_content("false", "application/json; charset=utf-8");
		}
	});

	svr->Delete(R"(/api/1.0/signals/(\s+))", [&](const Request& req, Response& res) {
		std::string zone_id = req.matches[1];

		bool ret = srv_keyboard->remove_override(zone_id);

		res.set_content(ret ? "true" : "false", "application/json; charset=utf-8");
	});

	svr->Get("/config.json", [&](const Request& req, Response& res) {
		std::ifstream file( "config.json");

		std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		res.set_content(data, "application/json; charset=utf-8");
	});

	svr->Post("/config.json", [&](const Request& req, Response& res) {
		try {
			nlohmann::json j = nlohmann::json::parse(req.body);
			srv_keyboard->read_config(j);


			std::ofstream file("config.json");
			file << req.body;
			file.close();

			res.set_content("true", "application/json; charset=utf-8");
		} catch (std::exception) {
			res.set_content("false", "application/json; charset=utf-8");
		}
	});

	svr->listen("localhost", 27301);

	return svr;
}


void start_server(const std::shared_ptr<DAS::Keyboard>& keyboard) {
	srv_keyboard = keyboard;
	server_thread = std::thread ([&] {
		server = run_server();
	});
}

void stop_server() {
	server->stop();
}

