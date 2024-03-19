#pragma once
#include "shader.hpp"
#include <vector>

class Mesh{
    public:
    Mesh(const float* data, int vertexCount);
    ~Mesh();
    // void addVertexAttrib(int size, const char* name);
    void attachShader(Shader* shader, std::vector<std::pair<int,const char*>> attributes);
    private:
    unsigned int vao, vbo;
    Shader* shader = nullptr;
};