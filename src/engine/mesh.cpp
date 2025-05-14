#include "mesh.hpp"
#include <glad/glad.h>
#include <iostream>

// Mesh::Mesh(float* data, int vCount, int vSize){
//     glGenVertexArrays(1, &vao);
//     glGenBuffers(1, &vbo);
//     glBindVertexArray(vao);
//     glBindBuffer(GL_ARRAY_BUFFER, vbo);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * vertexSize, data, GL_STATIC_DRAW);
//     vertexCount = vCount; vertexSize = vSize;
//     vertexAttribOffset = 0;
// }

Mesh::Mesh(std::vector<float> &data, std::vector<int> &attribSizes)
{
    vertexSize = 0;
    for(float size : attribSizes) vertexSize += size;
    // for(int i = 0; i < data.size(); i++) std::cout << data.data()[i] << std::endl;
    vertexCount = data.size() / vertexSize;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    // vertexCount = vCount; vertexSize = vSize;
    int count = 0; int offset = 0;
    for(float size : attribSizes){
        glVertexAttribPointer(count, size, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(count);
        count++;
        offset += size;
    }
    // vertexCount = data.size() / count;
    // vertexSize = offset;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

// void Mesh::addVertexAttrib(int size)
// {
//     glVertexAttribPointer(location, count, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(vertexAttribOffset * sizeof(float)));
//     vertexAttribOffset += count;
//     if(vertexAttribOffset > vertexSize) std::cout << "Too many vertex attributes!" << std::endl;
// }
