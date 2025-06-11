#pragma once

#include <glm/vec2.hpp>

namespace pokedex {
	namespace Window {
		using RenderCallback = void(*)(const float deltaTime);
		using WindowFocusCallback = void(*)(const int focused);
		using CursorEnterCallback = void(*)(const int entered);
		using CursorPosCallback = void(*)(const double xpos, const double ypos);
		using MouseButtonCallback = void(*)(const int button, const int action, const int mods);
		using ScrollCallback = void(*)(const double xoffset, const double yoffset);
		using KeyCallback = void(*)(const int key, const int scancode, const int action, const int mods);
		using CharCallback = void(*)(const unsigned int codepoint);

		int construct(const glm::ivec2& size, const char* title);
		int destruct();

		void frame();

		bool wantsShutdown();
		void requestShutdown();
		
		RenderCallback setOnRenderCallback(RenderCallback callback);
		WindowFocusCallback setOnWindowFocusCallback(WindowFocusCallback callback);
		CursorEnterCallback setOnCursorEnterCallback(CursorEnterCallback callback);
		CursorPosCallback setOnCursorPosCallback(CursorPosCallback callback);
		MouseButtonCallback setOnMouseButtonCallback(MouseButtonCallback callback);
		ScrollCallback setOnScrollCallback(ScrollCallback callback);
		KeyCallback setOnKeyCallback(KeyCallback callback);
		CharCallback setOnCharCallback(CharCallback callback);
	}
}
