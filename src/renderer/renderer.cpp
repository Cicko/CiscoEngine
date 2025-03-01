#include "renderer.hpp"
#include <cmath>

void Renderer::initRenderer() {
    shaderProgram = initLightingShader();
    glEnable(GL_DEPTH_TEST);

    // Projection matrix (FOV 45°, near 0.1, far 10)
    float fov = 45.0f * M_PI / 180.0f;
    projection[0] = 1.0f / tanf(fov / 2.0f) * (600.0f / 800.0f); // Aspect ratio HEIGHT/WIDTH
    projection[1] = 0.0f; projection[2] = 0.0f; projection[3] = 0.0f;
    projection[4] = 0.0f;
    projection[5] = 1.0f / tanf(fov / 2.0f);
    projection[6] = 0.0f; projection[7] = 0.0f;
    projection[8] = 0.0f; projection[9] = 0.0f;
    projection[10] = -(10.0f + 0.1f) / (10.0f - 0.1f);
    projection[11] = -1.0f;
    projection[12] = 0.0f; projection[13] = 0.0f;
    projection[14] = -2.0f * 10.0f * 0.1f / (10.0f - 0.1f);
    projection[15] = 0.0f;
}

void Renderer::render(const Scene& scene, const Camera& camera, float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    setupLighting(shaderProgram);

    float view[16];
    camera.getViewMatrix(view);

    // Floor (white wireframe)
    float floorModel[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, floorModel);
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 1.0f, 1.0f);
    glBindVertexArray(scene.floorVAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(0);
}