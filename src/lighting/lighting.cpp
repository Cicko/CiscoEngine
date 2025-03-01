#include "lighting.hpp"
#include <iostream>  // For std::cerr and std::endl
#include <cstddef>   // For nullptr (optional, but included for clarity)

// Vertex Shader (includes position and normal for lighting)
const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec3 aNormal;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "    FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\n";

// Fragment Shader (ambient + diffuse lighting)
const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "uniform vec3 objectColor;\n"
    "uniform vec3 lightDir;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 ambientColor;\n"
    "void main() {\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 dir = normalize(-lightDir);\n"
    "    float diff = max(dot(norm, dir), 0.0);\n"
    "    vec3 diffuse = diff * lightColor;\n"
    "    vec3 ambient = ambientColor;\n"
    "    vec3 result = (ambient + diffuse) * objectColor;\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}\n";

unsigned int initLightingShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex Shader Error: " << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment Shader Error: " << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program Error: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void setupLighting(unsigned int shaderProgram) {
    float lightDir[3] = {0.0f, -1.0f, -0.5f};
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, lightDir);

    float lightColor[3] = {1.0f, 1.0f, 1.0f};
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, lightColor);

    // Boost ambient to near-full strength
    float ambientColor[3] = {0.8f, 0.8f, 0.8f}; // 80% brightness
    glUniform3fv(glGetUniformLocation(shaderProgram, "ambientColor"), 1, ambientColor);
}