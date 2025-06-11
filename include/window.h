#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <glm/vec2.hpp>

#include <stdexcept>

namespace pokedex {
	class Window {
	public:
		using renderCallback = void(*)(Window& window, const float deltaTime);
		using windowFocusCallback = void(*)(Window& window, const int focused);
		using cursorEnterCallback = void(*)(Window& window, const int entered);
		using cursorPosCallback = void(*)(Window& window, const glm::vec2& pos);
		using mouseButtonCallback = void(*)(Window& window, const int button, const int action, const int mods);
		using scrollCallback = void(*)(Window& window, const glm::vec2& yoffset);
		using keyCallback = void(*)(Window& window, const int key, const int scancode, const int action, const int mods);
		using charCallback = void(*)(Window& window, const unsigned int codepoint);

		Window(const glm::ivec2& size, const char* title);
		~Window();

		void render();

		bool wantsShutdown();
		void requestShutdown();

		struct WindowCreationError : public std::runtime_error {
			WindowCreationError(const char* const message) throw() : std::runtime_error(message) {};
		};

		renderCallback set_renderCallback(renderCallback callback);
		windowFocusCallback set_windowFocusCallback(windowFocusCallback callback);
		cursorEnterCallback set_cursorEnterCallback(cursorEnterCallback callback);
		cursorPosCallback set_cursorPosCallback(cursorPosCallback callback);
		mouseButtonCallback set_mouseButtonCallback(mouseButtonCallback callback);
		scrollCallback set_scrollCallback(scrollCallback callback);
		keyCallback set_keyCallback(keyCallback callback);
		charCallback set_charCallback(charCallback callback);

	private:
		void preRender();
		void postRender();

		GLFWwindow* m_window = nullptr;
		ImGuiContext* m_context = nullptr;
		const char* m_title = nullptr;

		renderCallback m_renderCallback = nullptr;
		windowFocusCallback m_windowFocusCallback = nullptr;
		cursorEnterCallback m_cursorEnterCallback = nullptr;
		cursorPosCallback m_cursorPosCallback = nullptr;
		mouseButtonCallback m_mouseButtonCallback = nullptr;
		scrollCallback m_scrollCallback = nullptr;
		keyCallback m_keyCallback = nullptr;
		charCallback m_charCallback = nullptr;

		static void s_windowFocusCallback(GLFWwindow* glfwWindow, int focused);
		static void s_cursorEnterCallback(GLFWwindow* glfwWindow, int entered);
		static void s_cursorPosCallback(GLFWwindow* glfwWindow, double xpos, double ypos);
		static void s_mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
		static void s_scrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
		static void s_keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
		static void s_charCallback(GLFWwindow* glfwWindow, unsigned int codepoint);
	};
}
