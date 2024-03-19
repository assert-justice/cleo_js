#include "mesh.hpp"
#include <glad/glad.h>
#include <iostream>

Mesh::Mesh(const float* data, int vertexCount){
    // When created, a mesh isn't ready to be drawn which is arguably bad style. The attachShader method must be called before it is ready.
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexCount, data, GL_STATIC_DRAW);
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::attachShader(Shader* shader, std::vector<std::pair<int,const char*>> attributes){
    this->shader = shader;
    int stride = 0;
    int idx;
    for(idx = 0; idx < attributes.size(); idx++){
        stride += attributes[idx].first;
    }
    int offset = 0;
    for(idx = 0; idx < attributes.size(); idx++){
        auto loc = shader->getAttribLocation(attributes[idx].second);
        auto count = attributes[idx].first;
        glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
        offset += count;
    }
}