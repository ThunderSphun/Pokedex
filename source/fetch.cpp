#include "fetch.h"

#include <curl/curl.h>

#include <iostream>
#include <sstream>

namespace pokedex {
	namespace Fetch {
		constexpr const char* c_apiEndpoint = "https://pokeapi.co/api/v2/";

		bool g_initialized;
		CURL* g_curl;

		std::string fetch(const std::string_view& url);
		const Pokemon getPokemon(const nlohmann::json& fetchData);
	}
}

int pokedex::Fetch::init() {
	if (g_initialized)
		return -1;

	CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
	if (res != CURLE_OK) {
		std::cerr << "Could not initialize curl: " << curl_easy_strerror(res) << std::endl;
		return -2;
	}
	g_curl = curl_easy_init();
	if (g_curl == nullptr) {
		curl_global_cleanup();
		return -3;
	}

	g_initialized = true;

	return 0;
}

void pokedex::Fetch::deinit() {
	if (!g_initialized)
		return;

	curl_easy_cleanup(g_curl);
	curl_global_cleanup();
	g_initialized = false;
}

std::string pokedex::Fetch::fetch(const std::string_view& urlPath) {
	if (!g_initialized)
		return "";

	std::stringstream ss{};

	ss
		<< c_apiEndpoint
		<< urlPath;

	std::string url = ss.str();

	curl_easy_setopt(g_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(g_curl, CURLOPT_HTTPGET, 1L);

	std::string result{};
	curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, &result);
	curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, static_cast<size_t(*)(void*, size_t, size_t, void*)>([](void* ptr, size_t size, size_t nmemb, void* data) {
		reinterpret_cast<std::string*>(data)->append(reinterpret_cast<char*>(ptr), size * nmemb);

		return size * nmemb;
	}));

	CURLcode res = curl_easy_perform(g_curl);
	if (res != CURLE_OK) {
		std::cerr << "Could not send request: " << curl_easy_strerror(res) << std::endl;

		return "";
	}

	return result;
}

const pokedex::Pokemon pokedex::Fetch::getPokemon(const nlohmann::json& fetchedContent) {
	std::cout << fetchedContent.dump(1) << std::endl;
	return {};
}

const pokedex::Pokemon pokedex::Fetch::getPokemon(uint16_t dexNr) {
	std::string result = fetch(std::string("pokemon/").append(std::to_string(dexNr)));
	nlohmann::json jsonData = nlohmann::json::parse(result);
	Pokemon pokemon = getPokemon(jsonData);
	return pokemon;
}

const pokedex::Pokemon pokedex::Fetch::getPokemon(const std::string_view& monName) {
	std::string result = fetch(std::string("pokemon/").append(monName));
	nlohmann::json jsonData = nlohmann::json::parse(result);
	Pokemon pokemon = getPokemon(jsonData);
	return pokemon;
}
