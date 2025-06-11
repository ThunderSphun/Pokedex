#include "window.h"

#include <imgui.h>
#include <glad/gl.h>
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

	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW Error " << error << ": " << description << std::endl;
	});

	if (!glfwInit())
		return -1;

	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		pokedex::Window window[2] = { pokedex::Window({640, 480}, "A"), pokedex::Window({640, 480}, "B") };

		window[0].set_keyCallback([](pokedex::Window& window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				window.requestShutdown();
		});
		window[1].set_keyCallback([](pokedex::Window& window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				window.requestShutdown();
		});

		while (!window[0].wantsShutdown() && !window[1].wantsShutdown()) {
			glfwPollEvents();

			window[0].render();
			window[1].render();
		}
	}

	glfwTerminate();

	return 0;
}
