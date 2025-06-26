#include "window.h"
#include "fetch.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <imgui.h>

int main() {
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
		if (pokedex::Fetch::init() != 0) {
			pokedex::Fetch::deinit();
			return -1;
		}
		if (pokedex::Window::construct({ 640, 480 }, "window") != 0) {
			pokedex::Window::destruct();
			pokedex::Fetch::deinit();
			return -2;
		}

		pokedex::Window::setOnKeyCallback([](const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				pokedex::Window::requestShutdown();
		});

		pokedex::Window::setOnFrameRenderCallback([](const double frameTime, const uint64_t frameCount) {
			if (ImGui::Begin("window")) {
				ImGui::Text("FrameCount: %lu", frameCount);
				ImGui::Text("FrameTime: %lf", frameTime);
			} ImGui::End();
		});

		pokedex::Fetch::getPokemon(1);
		pokedex::Fetch::getPokemon("bulbasaur");

		while (!pokedex::Window::wantsShutdown()) {
			glfwPollEvents();

			pokedex::Window::frame();
		}

		pokedex::Window::destruct();
		pokedex::Fetch::deinit();
	}

	return 0;
}
