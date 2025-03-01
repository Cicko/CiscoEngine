#include "camera.hpp"
#include <cmath>

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        for (int i = 0; i < 3; i++) pos[i] += cameraSpeed * front[i];
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        for (int i = 0; i < 3; i++) pos[i] -= cameraSpeed * front[i];
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        float right[3] = {front[2], 0.0f, -front[0]};
        float len = sqrtf(right[0] * right[0] + right[2] * right[2]);
        for (int i = 0; i < 3; i++) pos[i] += cameraSpeed * right[i] / len;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        float right[3] = {front[2], 0.0f, -front[0]};
        float len = sqrtf(right[0] * right[0] + right[2] * right[2]);
        for (int i = 0; i < 3; i++) pos[i] -= cameraSpeed * right[i] / len;
    }
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    float frontX = cosf(yaw * M_PI / 180.0f) * cosf(pitch * M_PI / 180.0f);
    float frontY = sinf(pitch * M_PI / 180.0f);
    float frontZ = sinf(yaw * M_PI / 180.0f) * cosf(pitch * M_PI / 180.0f);
    float len = sqrtf(frontX * frontX + frontY * frontY + frontZ * frontZ);
    front[0] = frontX / len;
    front[1] = frontY / len;
    front[2] = frontZ / len;
}

void Camera::getViewMatrix(float* view) const {
    float target[3] = {pos[0] + front[0], pos[1] + front[1], pos[2] + front[2]};
    float zaxis[3] = {front[0], front[1], front[2]};
    float xaxis[3] = {up[2] * zaxis[1] - up[1] * zaxis[2], up[0] * zaxis[2] - up[2] * zaxis[0], up[1] * zaxis[0] - up[0] * zaxis[1]};
    float len = sqrtf(xaxis[0] * xaxis[0] + xaxis[1] * xaxis[1] + xaxis[2] * xaxis[2]);
    xaxis[0] /= len; xaxis[1] /= len; xaxis[2] /= len;
    float yaxis[3] = {zaxis[1] * xaxis[2] - zaxis[2] * xaxis[1], zaxis[2] * xaxis[0] - zaxis[0] * xaxis[2], zaxis[0] * xaxis[1] - zaxis[1] * xaxis[0]};
    view[0] = xaxis[0]; view[4] = xaxis[1]; view[8] = xaxis[2]; view[12] = -(xaxis[0] * pos[0] + xaxis[1] * pos[1] + xaxis[2] * pos[2]);
    view[1] = yaxis[0]; view[5] = yaxis[1]; view[9] = yaxis[2]; view[13] = -(yaxis[0] * pos[0] + yaxis[1] * pos[1] + yaxis[2] * pos[2]);
    view[2] = -zaxis[0]; view[6] = -zaxis[1]; view[10] = -zaxis[2]; view[14] = -(-zaxis[0] * pos[0] + -zaxis[1] * pos[1] + -zaxis[2] * pos[2]);
    view[3] = 0.0f; view[7] = 0.0f; view[11] = 0.0f; view[15] = 1.0f;
}