cmake_minimum_required(VERSION 3.28)
include(FetchContent)
project(pokedex)

set(VS_STARTUP_PROJECT ${PROJECT_NAME})

find_package(OpenGL REQUIRED)

FetchContent_Declare(glfw3	GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(glad	GIT_REPOSITORY https://github.com/Dav1dde/glad.git	GIT_TAG origin/glad2)
FetchContent_Declare(glm	GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_Declare(imgui	GIT_REPOSITORY https://github.com/ocornut/imgui.git	GIT_TAG origin/docking)
FetchContent_Declare(curl	GIT_REPOSITORY https://github.com/curl/curl.git)
FetchContent_Declare(json	GIT_REPOSITORY https://github.com/nlohmann/json.git)

# disabling lib checks for curl
set(CURL_ZLIB OFF CACHE STRING "Build curl with ZLIB support (AUTO, ON or OFF)")
set(CURL_BROTLI OFF CACHE STRING "Build curl with brotli support (AUTO, ON or OFF)")
set(CURL_ZSTD OFF CACHE STRING "Build curl with zstd support (AUTO, ON or OFF)")

set(CURL_USE_LIBPSL OFF CACHE BOOL "Use libpsl")
set(CURL_USE_LIBSSH2 OFF CACHE BOOL "Use libssh2")

set(USE_NGHTTP2 OFF CACHE BOOL "Use nghttp2 library")
set(USE_LIBIDN2 OFF CACHE BOOL "Use libidn2 for IDN support")

# building glfw examples
set(GLFW_BUILD_EXAMPLES ON)

# stop building curl targets
set(BUILD_EXAMPLES OFF CACHE BOOL "Build libcurl examples")
set(BUILD_CURL_EXE OFF CACHE BOOL "Build curl executable")
set(BUILD_TESTING OFF CACHE BOOL "Build tests")

FetchContent_MakeAvailable(glfw3 glad glm imgui curl json)

# generate glad
add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
glad_add_library(glad REPRODUCIBLE LOADER API gl:core=4.3)

# create DearImGui library
add_library(Dear_ImGui STATIC
	# source
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_demo.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp

	# cpp extension
	${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp

	# backend
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)
# link DearImGui library
target_include_directories(Dear_ImGui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/misc/cpp ${imgui_SOURCE_DIR}/backends)
target_link_libraries(Dear_ImGui glfw)

#
# actually create this project
#

add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE
	source/window.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_link_libraries(${PROJECT_NAME}
	glfw glad glm Dear_ImGui CURL::libcurl nlohmann_json::nlohmann_json)

# target_compile_options(${PROJECT_NAME} PUBLIC /P)

add_executable(tmp tmp.cpp)
target_compile_features(tmp PUBLIC cxx_std_23)
target_link_libraries(tmp glfw glad glm Dear_ImGui CURL::libcurl nlohmann_json::nlohmann_json)
