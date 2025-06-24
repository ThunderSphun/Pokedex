#include "window.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <imgui.h>

int main() {
	{
		curl_global_init(CURL_GLOBAL_ALL);
		CURL* curl = curl_easy_init();

		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, "https://pokeapi.co/api/v2/pokemon/ditto");
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

			std::string result{};
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, static_cast<size_t(*)(void*, size_t, size_t, void*)>([](void* ptr, size_t size, size_t nmemb, void* data) {
				reinterpret_cast<std::string*>(data)->append(reinterpret_cast<char*>(ptr), size * nmemb);

				return size * nmemb;
			}));

			CURLcode res = curl_easy_perform(curl);

			if (res == CURLE_OK) {
				nlohmann::json pokeJson = nlohmann::json::parse(result);
				std::cout << "Got data:" << std::endl << pokeJson.dump(1, '\t') << std::endl;
			}
			else
				std::cerr << "something went wrong: " << curl_easy_strerror(res) << std::endl;

			curl_easy_cleanup(curl);
		}

		curl_global_cleanup();

		std::cin.get();

		return 0;
	}

	{
		nlohmann::json json = {
			{"happy", true},
			{"pi", 3.141},
			{"obj", {
				{"happy", true},
				{"pi", 3.1415926},
			}},
		};

		std::cout << std::endl << json.dump(1, '\t') << std::endl;
	}

	{
		if (pokedex::Window::construct({ 640, 480 }, "window") != 0) {
			pokedex::Window::destruct();
			return -1;
		}

		pokedex::Window::setOnKeyCallback([](const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				pokedex::Window::requestShutdown();
		});

		pokedex::Window::setOnFrameRenderCallback([](const double frameTime, const uint64_t frameCount) {
			if (ImGui::Begin("window")) {
				ImGui::Text("FrameCount: %lli", frameCount);
				ImGui::Text("FrameTime: %llf", frameTime);
			} ImGui::End();
		});

		while (!pokedex::Window::wantsShutdown()) {
			glfwPollEvents();

			pokedex::Window::frame();
		}

		pokedex::Window::destruct();
	}

	return 0;
}
