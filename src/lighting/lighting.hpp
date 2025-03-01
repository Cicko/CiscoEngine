#ifndef LIGHTING_HPP
#define LIGHTING_HPP

#include <glad/glad.h>

// Vertex Shader with lighting
extern const char* vertexShaderSource;

// Fragment Shader with basic global illumination (ambient + diffuse)
extern const char* fragmentShaderSource;

// Initialize shader program
unsigned int initLightingShader();

// Set up lighting uniforms
void setupLighting(unsigned int shaderProgram);

#endif