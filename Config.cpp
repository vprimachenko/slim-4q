//
// Created by Valerij Primachenko on 20-06-30.
//

#include "Config.h"
#include "deps/magic_enum.hpp"

std::map<DAS::Key, Config> parse_config(const nlohmann::json &j) {
		// FIXME broken af
	std::map<DAS::Key, Config> configs;

	for (auto& [_key, value] : j.items()) {

		auto key = magic_enum::enum_cast<DAS::Key>(_key);
		if (!key.has_value()) { continue; }

		configs[key.value()] = value.get<Config>();
	}

	return std::move(configs);
}

//
//void to_json(json& j, const __KeyConfig& p) {
//    j = json{{"name", p.name}, {"address", p.address}, {"age", p.age}};
//}
void from_json(const nlohmann::json& j, Config& p) {
	if (j.contains("passive")) {
		j.at("passive").get_to(p.passive);
	}
	if (j.contains("active")) {
		j.at("active").get_to(p.active);
	}
}

void from_json(const nlohmann::json& j, KeyConfig& p) {
	p[{}] = {DAS::Effect::PASSIVE_NONE, 0, 0, 0};

	for (auto& [_desiredState, cf] : j.items()) {
		auto _state = split(_desiredState, '+');
		std::bitset<5> desiredState;

		desiredState[0] = std::find(_state.cbegin(), _state.cend(), "NUM_LOCK") != _state.cend();
		desiredState[1] = std::find(_state.cbegin(), _state.cend(), "CAPS_LOCK") != _state.cend();
		desiredState[2] = std::find(_state.cbegin(), _state.cend(), "CONTROL") != _state.cend();
		desiredState[3] = std::find(_state.cbegin(), _state.cend(), "MENU") != _state.cend();
		desiredState[4] = std::find(_state.cbegin(), _state.cend(), "SHIFT") != _state.cend();

		p[desiredState] = cf.get<EffectConfig>();
	}
}

void from_json(const nlohmann::json& j, EffectConfig& p) {
	auto effect = magic_enum::enum_cast<DAS::Effect>(j.at("effect").get<std::string>());

	if (effect.has_value()) {
		p.fx = effect.value();
	} else {
		p.fx = DAS::Effect::PASSIVE_SET;
	}

	j.at("r").get_to(p.r);
	j.at("g").get_to(p.g);
	j.at("b").get_to(p.b);
}

bool operator==(const Config &lhs, const Config &rhs) {
	return lhs.passive == rhs.passive && lhs.active == rhs.active;
}

bool operator==(const EffectConfig& lhs, const EffectConfig& rhs ) {
	return  lhs.fx == rhs.fx &&
	        lhs.r  == rhs.r &&
	        lhs.g  == rhs.g &&
	        lhs.b  == rhs.b;
}

std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> strings;
    size_t start;
    size_t end = 0;
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find(delim, start);
        strings.push_back(str.substr(start, end - start));
    }
    return strings;
}
//template<DAS::Effect>
//DAS::Effect nlohmann::basic_json::get() const {
//
//    return JSONSerializer<ValueType>::from_json(*this);
//}
