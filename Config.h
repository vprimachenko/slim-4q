//
// Created by Valerij Primachenko on 20-06-30.
//

#ifndef DAS_CONFIG_H
#define DAS_CONFIG_H

#include <bitset>
#include <map>
#include "json/json.hpp"
#include "keys.h"
#include "effect.h"

namespace std {
	template<size_t N>
	struct less<bitset<N>> {
		bool operator() (const bitset<N> &L, const bitset<N> &R) const {
			for(unsigned int i=0;i<L.size();i++)
				if(L.test(i)) {
					if(!R.test(i))return false;
				} else {
					if(R.test(i))return true;
				}
			return false; //same
		}
	};
}

struct EffectConfig {
	DAS::Effect fx;
	uint8_t r, g, b;
};

typedef std::map<std::bitset<5>, EffectConfig> KeyConfig;

struct Config {
	KeyConfig passive, active;
};

std::map<DAS::Key, Config> parse_config(const nlohmann::json &j);

bool operator==(const Config &lhs, const Config &rhs);
bool operator==(const EffectConfig &lhs, const EffectConfig &rhs);

void from_json(const nlohmann::json& j, Config& p);
void from_json(const nlohmann::json& j, KeyConfig& p);
void from_json(const nlohmann::json &j, EffectConfig &p);

std::vector<std::string> split(const std::string& str, char delim);

#endif //DAS_CONFIG_H
