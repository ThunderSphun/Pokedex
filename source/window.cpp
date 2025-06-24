#include "window.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>

namespace pokedex {
	namespace Window {
		void clearGlobals();

		bool g_initialized = false;
		GLFWwindow* g_window = nullptr;
		ImGuiContext* g_context = nullptr;
		uint64_t g_frameCount = 0;
		double g_prevTime = 0;
		
		FrameRenderCallback g_onFrameRender = nullptr;
		WindowFocusCallback g_onWindowFocus = nullptr;
		CursorEnterCallback g_onCursorEnter = nullptr;
		CursorPosCallback g_onCursorPos = nullptr;
		MouseButtonCallback g_onMouseButton = nullptr;
		ScrollCallback g_onScroll = nullptr;
		KeyCallback g_onKey = nullptr;
		CharCallback g_onChar = nullptr;

		void setCallbacks();
		int initDearImGui();

		void preFrame();
		void postFrame();
	}
}

void pokedex::Window::clearGlobals() {
	g_initialized = false;
	g_window = nullptr;
	g_context = nullptr;
	g_frameCount = 0;
	g_prevTime = 0;

	g_onFrameRender = nullptr;
	g_onWindowFocus = nullptr;
	g_onCursorEnter = nullptr;
	g_onCursorPos = nullptr;
	g_onMouseButton = nullptr;
	g_onScroll = nullptr;
	g_onKey = nullptr;
	g_onChar = nullptr;
}

int pokedex::Window::construct(const glm::ivec2& size, const char* title) {
	if (g_initialized)
		return -1;

	clearGlobals();

	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW Error " << error << ": " << description << std::endl;
	});

	if (!glfwInit())
		return -2;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	g_window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
	if (g_window == nullptr) {
		clearGlobals();
		glfwTerminate();
		return -3;
	}

	glfwMakeContextCurrent(g_window);
	glfwSwapInterval(1);
	gladLoadGL(glfwGetProcAddress);

	setCallbacks();

	int err = initDearImGui();
	if (err != 0) {
		glfwDestroyWindow(g_window);
		clearGlobals();
		glfwTerminate();
		return err < 0 ? err - 3 : err;
	}

	g_initialized = true;

	return 0;
}

int pokedex::Window::destruct() {
	if (!g_initialized)
		return -1;

	ImGui::SetCurrentContext(g_context);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(g_context);

	glfwDestroyWindow(g_window);

	glfwTerminate();

	return 0;
}

void pokedex::Window::frame() {
	if (!g_initialized)
		return;

	preFrame();

	double time = glfwGetTime();

	if (g_onFrameRender)
		g_onFrameRender(time - g_prevTime, g_frameCount++);

	g_prevTime = time;

	ImGui::ShowDemoWindow();

	postFrame();
}

void pokedex::Window::preFrame() {
	glfwMakeContextCurrent(g_window);
	ImGui::SetCurrentContext(g_context);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT);
}

void pokedex::Window::postFrame() {
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* old = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(old);
	}

	glfwSwapBuffers(g_window);
}

bool pokedex::Window::wantsShutdown() {
	return g_initialized || glfwWindowShouldClose(g_window);
}

void pokedex::Window::requestShutdown() {
	if (g_initialized)
		glfwSetWindowShouldClose(g_window, GLFW_TRUE);
}

#define CALLBACK_SETTER(name) \
pokedex::Window::name##Callback pokedex::Window::setOn##name##Callback(name##Callback callback) { \
	if (!g_initialized) \
		return nullptr; \
	name##Callback oldCallback = g_on##name; \
	g_on##name = callback; \
	return oldCallback; \
}

CALLBACK_SETTER(FrameRender);
CALLBACK_SETTER(WindowFocus);
CALLBACK_SETTER(CursorEnter);
CALLBACK_SETTER(CursorPos);
CALLBACK_SETTER(MouseButton);
CALLBACK_SETTER(Scroll);
CALLBACK_SETTER(Key);
CALLBACK_SETTER(Char);

#undef CALLBACK_SETTER

void pokedex::Window::setCallbacks() {
#define CALLBACK_PASSTHROUGH(name) \
	glfwSet##name##Callback(g_window, [](GLFWwindow* window, auto... args) { \
	ImGuiContext* old = ImGui::GetCurrentContext(); \
	ImGui::SetCurrentContext(g_context); \
	ImGui_ImplGlfw_##name##Callback(window, args...); \
 \
	if (g_on##name) \
		g_on##name(args...); \
 \
	ImGui::SetCurrentContext(old); \
})

	CALLBACK_PASSTHROUGH(WindowFocus);
	CALLBACK_PASSTHROUGH(CursorEnter);
	CALLBACK_PASSTHROUGH(CursorPos);
	CALLBACK_PASSTHROUGH(MouseButton);
	CALLBACK_PASSTHROUGH(Scroll);
	CALLBACK_PASSTHROUGH(Key);
	CALLBACK_PASSTHROUGH(Char);
	glfwSetMonitorCallback(nullptr);

#undef CALLBACK_PASSTHROUGH
}

int pokedex::Window::initDearImGui() {
	if (!IMGUI_CHECKVERSION())
		return -1;

	g_context = ImGui::CreateContext();
	ImGui::SetCurrentContext(g_context);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	if (!ImGui_ImplGlfw_InitForOpenGL(g_window, false)) {
		ImGui::DestroyContext();
		return -2;
	}
	if (!ImGui_ImplOpenGL3_Init("#version 430")) {
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return -3;
	}

	return 0;
}
