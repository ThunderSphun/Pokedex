#include "window.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

int main() {
	{
		CURL* curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

			std::string result{};
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t(*)(void*, size_t, size_t, void*)>([](void* ptr, size_t size, size_t nmemb, void* data) {
				reinterpret_cast<std::string*>(data)->append(reinterpret_cast<char*>(ptr), size * nmemb);

				return size * nmemb;
				}));

			CURLcode res = curl_easy_perform(curl);

			if (res == CURLE_OK)
				std::cout << "Got data:" << std::endl << result << std::endl;

			curl_easy_cleanup(curl);
		}
	}

	{
		nlohmann::json json = {
			{"happy", true},
			{"pi", 3.141},
		};

		std::cout << std::endl << json.dump(1, '\t') << std::endl;
	}

	{
		if (pokedex::Window::construct({ 640, 480 }, "window") != 0) {
			pokedex::Window::destruct();
			return -1;
		}

		pokedex::Window::setOnKeyCallback([](const int key, const int scancode, const int action, const int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				pokedex::Window::requestShutdown();
		});

		while (!pokedex::Window::wantsShutdown()) {
			glfwPollEvents();

			pokedex::Window::frame();
		}

		pokedex::Window::destruct();
	}

	return 0;
}
