#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/shader.hpp"
#include "scene/scene.hpp"
#include "renderer/renderer.hpp"
#include <iostream>
#include <string>
#include <cmath>

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Cisco Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Camera camera;
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        static_cast<Camera*>(glfwGetWindowUserPointer(w))->mouseCallback(w, x, y);
    });
    glfwSetWindowUserPointer(window, &camera);

    // TODO: Check if to use this.
    // Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    
    Scene scene;
    scene.initScene();

    // Add objects
    float pos1[3] = {0.0f, 0.0f, 0.0f};  // Cube at origin
    float pos2[3] = {2.0f, 0.0f, 2.0f};  // Another cube offset
    
    //scene.add("obj/cube.obj", pos1);
    //scene.add("obj/cube.obj", pos2);      // Add as many as you want


    Renderer renderer;
    renderer.initRenderer();

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.processInput(window, deltaTime);
        renderer.render(scene, camera, deltaTime);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene.cleanupScene();
    glDeleteProgram(renderer.shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}