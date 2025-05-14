#pragma once
#include "shader.hpp"
#include <vector>

class Mesh{
    public:
    // Mesh(float* data, int vertexCount, int vertexSize);
    Mesh(std::vector<float> &data, std::vector<int> &attribSizes);
    ~Mesh();
    void draw();
    // void addVertexAttrib(int size);
    // void attachShader(Shader* shader, std::vector<std::pair<int,const char*>> attributes);
    private:
    unsigned int vao, vbo;
    int vertexCount, vertexSize;//, vertexAttribOffset, vertexAttribCount;
};