#pragma once

#include <glm/vec2.hpp>

#include <cstdint>

namespace pokedex {
	namespace Window {
		using FrameRenderCallback = void(*)(const double deltaTime, const uint64_t frameCount);
		using WindowFocusCallback = void(*)(const int32_t focused);
		using CursorEnterCallback = void(*)(const int32_t entered);
		using CursorPosCallback = void(*)(const double xpos, const double ypos);
		using MouseButtonCallback = void(*)(const int32_t button, const int32_t action, const int32_t mods);
		using ScrollCallback = void(*)(const double xoffset, const double yoffset);
		using KeyCallback = void(*)(const int32_t key, const int32_t scancode, const int32_t action, const int32_t mods);
		using CharCallback = void(*)(const uint32_t codepoint);

		int construct(const glm::ivec2& size, const char* title);
		int destruct();

		void frame();

		bool wantsShutdown();
		void requestShutdown();
		
		FrameRenderCallback setOnFrameRenderCallback(FrameRenderCallback callback);
		WindowFocusCallback setOnWindowFocusCallback(WindowFocusCallback callback);
		CursorEnterCallback setOnCursorEnterCallback(CursorEnterCallback callback);
		CursorPosCallback setOnCursorPosCallback(CursorPosCallback callback);
		MouseButtonCallback setOnMouseButtonCallback(MouseButtonCallback callback);
		ScrollCallback setOnScrollCallback(ScrollCallback callback);
		KeyCallback setOnKeyCallback(KeyCallback callback);
		CharCallback setOnCharCallback(CharCallback callback);
	}
}
