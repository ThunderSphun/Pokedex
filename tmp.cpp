#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	struct windowContext {
		GLFWwindow* window;
		ImGuiContext* context;
	};

	windowContext windows[2] = {};

	for (int i = 0; i < 2; i++) {
		windows[i].window = glfwCreateWindow(640, 480, "title", nullptr, nullptr);
		if (windows[i].window == nullptr) {
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(windows[i].window);
		glfwSwapInterval(1);

		windows[i].context = ImGui::CreateContext();
		ImGui::SetCurrentContext(windows[i].context);
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(windows[i].window, false);
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	for (int i = 0; i < 2; i++) { // works
	//for (int i = 1; i >= 0; i--) { // doesn't work
		ImGui::SetCurrentContext(windows[i].context);
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		glfwDestroyWindow(windows[i].window);
	}

	glfwTerminate();

	return 0;
}
