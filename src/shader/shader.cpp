#include "shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Load shader sources
    std::string vertexSource = loadShaderSource(vertexPath);
    std::string fragmentSource = loadShaderSource(fragmentPath);

    // Compile shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    // Create and link program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    int success;
    char infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::use() const {
    glUseProgram(programID);
}

void Shader::setMat4(const std::string& name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

std::string Shader::loadShaderSource(const char* filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int Shader::compileShader(GLenum type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") 
                  << " shader compilation failed:\n" << infoLog << std::endl;
    }
    return shader;
}