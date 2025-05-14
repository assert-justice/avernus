#pragma once
#include <string>
// #include <glad/glad.h>
#include <glm/glm.hpp>

class Shader{
    public:
    unsigned int id;
    bool initialized = false;
    void initialize(bool* hasError, std::string vertexSrc, std::string fragmentSrc);
    // bool hasError = false;
    // Shader(std::string vertexSrc, std::string fragmentSrc);
    ~Shader();
    void use();
    void setUniform(std::string name, glm::mat4 matrix);
    void setUniform(int location, glm::mat4 matrix);
    // private:
    unsigned int getUniformLocation(const char* name);
    // unsigned int getAttribLocation(const char* name);
};