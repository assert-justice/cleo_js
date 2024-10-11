#include "mesh.hpp"
#include <glad/glad.h>
#include <iostream>

Mesh::Mesh(std::vector<float> data, std::vector<int> attributes){
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    int vertexSize = 0;
    for(auto attr : attributes){
        vertexSize += attr;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    int offset = 0;
    for(int idx = 0; idx < attributes.size(); idx++){
        auto count = attributes[idx];
        glVertexAttribPointer(idx, count, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void*)(offset * sizeof(float)));
        offset += count;
    }
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}