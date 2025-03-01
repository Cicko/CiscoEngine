#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>

struct Camera {
    float pos[3] = {0.0f, 1.0f, 3.0f};
    float front[3] = {0.0f, 0.0f, -1.0f};
    float up[3] = {0.0f, 1.0f, 0.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX = 400.0f; // Half of WIDTH
    float lastY = 300.0f; // Half of HEIGHT
    bool firstMouse = true;

    void processInput(GLFWwindow* window, float deltaTime);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void getViewMatrix(float* view) const;
};

#endif