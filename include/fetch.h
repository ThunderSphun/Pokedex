#pragma once

#include "data.h"

#include <nlohmann/json.hpp>

#include <cstdint>
#include <string_view>

namespace pokedex {
	namespace Fetch {
		int init();
		void deinit();

		const Pokemon getPokemon(uint16_t dexNr);
		const Pokemon getPokemon(const std::string_view& monName);
	}
}