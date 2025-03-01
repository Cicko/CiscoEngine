#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use() const;
    void setMat4(const std::string& name, const float* value) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int programID;
    std::string loadShaderSource(const char* filepath);
    unsigned int compileShader(GLenum type, const char* source);
};

#endif