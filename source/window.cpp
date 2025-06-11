#include "window.h"

#include <glad/gl.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using pokedex::Window;

Window::Window(const glm::ivec2& size, const char* title) : m_title(title) {
	m_window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
	if (m_window == nullptr)
		throw WindowCreationError("could not initialize window");

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(m_window, this);
	gladLoadGL(glfwGetProcAddress);

	m_context = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_context);
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

	ImGui_ImplGlfw_InitForOpenGL(m_window, false);
	ImGui_ImplOpenGL3_Init("#version 430");

	glfwSetWindowFocusCallback(m_window, s_windowFocusCallback);
	glfwSetCursorEnterCallback(m_window, s_cursorEnterCallback);
	glfwSetCursorPosCallback(m_window, s_cursorPosCallback);
	glfwSetMouseButtonCallback(m_window, s_mouseButtonCallback);
	glfwSetScrollCallback(m_window, s_scrollCallback);
	glfwSetKeyCallback(m_window, s_keyCallback);
	glfwSetCharCallback(m_window, s_charCallback);
	glfwSetMonitorCallback(nullptr);
}

Window::~Window() {
	ImGui::SetCurrentContext(m_context);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(m_context);

	glfwSetWindowUserPointer(m_window, nullptr);

	glfwDestroyWindow(m_window);
}

void Window::render() {
	preRender();

	if (m_renderCallback)
		m_renderCallback(*this, 0.0f);

	ImGui::PushID(m_window);
	ImGui::ShowDemoWindow();
	ImGui::PopID();

	postRender();
}

bool pokedex::Window::wantsShutdown() {
	return glfwWindowShouldClose(m_window);
}

void pokedex::Window::requestShutdown() {
	glfwSetWindowShouldClose(m_window, GLFW_TRUE);
}

void Window::preRender() {
	glfwMakeContextCurrent(m_window);
	ImGui::SetCurrentContext(m_context);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::postRender() {
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(m_window);
}

#define CALLBACK_SETTER(name) \
Window::name Window::set_##name(name callback) { \
	Window::name oldCallback = m_##name; \
	m_##name = callback; \
	return oldCallback; \
}

CALLBACK_SETTER(renderCallback);
CALLBACK_SETTER(windowFocusCallback);
CALLBACK_SETTER(cursorEnterCallback);
CALLBACK_SETTER(cursorPosCallback);
CALLBACK_SETTER(mouseButtonCallback);
CALLBACK_SETTER(scrollCallback);
CALLBACK_SETTER(keyCallback);
CALLBACK_SETTER(charCallback);

#undef CALLBACK_SETTER

void Window::s_windowFocusCallback(GLFWwindow* glfwWindow, int focused) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_WindowFocusCallback(glfwWindow, focused);
	if (window->m_windowFocusCallback)
		window->m_windowFocusCallback(*window, focused);
	ImGui::SetCurrentContext(old);
}

void Window::s_cursorEnterCallback(GLFWwindow* glfwWindow, int entered) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_CursorEnterCallback(glfwWindow, entered);
	if (window->m_cursorEnterCallback)
		window->m_cursorEnterCallback(*window, entered);
	ImGui::SetCurrentContext(old);
}

void Window::s_cursorPosCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_CursorPosCallback(glfwWindow, xpos, ypos);
	if (window->m_cursorPosCallback)
		window->m_cursorPosCallback(*window, { xpos, ypos });
	ImGui::SetCurrentContext(old);
}

void Window::s_mouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_MouseButtonCallback(glfwWindow, button, action, mods);
	if (window->m_mouseButtonCallback)
		window->m_mouseButtonCallback(*window, button, action, mods);
	ImGui::SetCurrentContext(old);
}

void Window::s_scrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_ScrollCallback(glfwWindow, xoffset, yoffset);
	if (window->m_scrollCallback)
		window->m_scrollCallback(*window, { xoffset, yoffset });
	ImGui::SetCurrentContext(old);
}

void Window::s_keyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_KeyCallback(glfwWindow, key, scancode, action, mods);
	if (window->m_keyCallback)
		window->m_keyCallback(*window, key, scancode, action, mods);
	ImGui::SetCurrentContext(old);
}

void Window::s_charCallback(GLFWwindow* glfwWindow, unsigned int codepoint) {
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (window == nullptr)
		return;

	ImGuiContext* old = ImGui::GetCurrentContext();
	ImGui::SetCurrentContext(window->m_context);
	ImGui_ImplGlfw_CharCallback(glfwWindow, codepoint);
	if (window->m_charCallback)
		window->m_charCallback(*window, codepoint);
	ImGui::SetCurrentContext(old);
}
