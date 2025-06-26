#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>

namespace pokedex {
	enum class Type {
		NORMAL, FIGHTING, FLYING, POISON,
		GROUND, ROCK, BUG, GHOST,
		STEEL, FIRE, WATER, GRASS,
		ELECTRIC, PSYCHIC, ICE, DRAGON,
		DARK, FAIRY, STELLAR, UNKNOWN,
		NONE,
	};

	struct Pokemon {
		uint16_t id;
		std::string name;
		Type types[2];
		std::string abilities[2];
		std::string hiddenAbility;

		struct {
			uint8_t hp;
			uint8_t attack;
			uint8_t defense;
			uint8_t specialAttack;
			uint8_t specialDefense;
			uint8_t speed;
		} stats;

		nlohmann::json rawData;
	};
}