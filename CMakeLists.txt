cmake_minimum_required(VERSION 3.10)
project(CiscoEngine)

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} src/main.cpp src/lighting/lighting.cpp src/scene/scene.cpp src/shader/shader.cpp src/camera/camera.cpp src/renderer/renderer.cpp src/glad.c)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

# Optional: Copy shaders to build directory (uncomment if needed)
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})