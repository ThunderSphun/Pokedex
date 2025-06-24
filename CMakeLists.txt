cmake_minimum_required(VERSION 3.28)
project(pokedex)

set(VS_STARTUP_PROJECT ${PROJECT_NAME})

find_package(OpenGL REQUIRED)

add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glad")
add_subdirectory("vendor/glm")
add_subdirectory("vendor/imgui")
add_subdirectory("vendor/curl")
add_subdirectory("vendor/json")

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
