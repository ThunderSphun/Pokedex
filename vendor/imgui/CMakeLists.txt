add_library(Dear_ImGui STATIC
	# source
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp

	# cpp extension
	imgui/misc/cpp/imgui_stdlib.cpp

	# backend
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/backends/imgui_impl_glfw.cpp
)
# link DearImGui library
target_include_directories(Dear_ImGui PUBLIC imgui imgui/misc/cpp imgui/backends)
target_link_libraries(Dear_ImGui glfw)
