#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include "../scene/scene.hpp"
#include "../camera/camera.hpp"
#include "../lighting/lighting.hpp"

struct Renderer {
    unsigned int shaderProgram;
    float projection[16];

    void initRenderer();
    void render(const Scene& scene, const Camera& camera, float deltaTime);
};

#endif