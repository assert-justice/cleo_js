#include "mesh.hpp"
#include <glad/glad.h>
#include <iostream>

Mesh::Mesh(Shader* shader, const float* data, int vertexCount, int vertexSize, std::vector<std::pair<int,const char*>> attributes){
    // Maybe check if the vertex count is an even multiple of three
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount * vertexSize, data, GL_STATIC_DRAW);
    int offset = 0;
    for(int idx = 0; idx < attributes.size(); idx++){
        auto loc = shader->getAttribLocation(attributes[idx].second);
        auto count = attributes[idx].first;
        glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset * sizeof(float)));
        offset += count;
        if(offset > vertexSize){
            // Should report this error somehow.
        }
    }
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}