#pragma once
#include "shader.hpp"
#include <vector>

class Mesh{
    public:
    Mesh(Shader* shader, const float* data, int vertexCount, int vertexSize, std::vector<std::pair<int,const char*>> attributes);
    ~Mesh();
    // void addVertexAttrib(int size, const char* name);
    // void attachShader(Shader* shader, std::vector<std::pair<int,const char*>> attributes);
    private:
    unsigned int vao, vbo;
    Shader* shader = nullptr;
};