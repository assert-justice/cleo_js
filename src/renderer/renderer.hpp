#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class Renderer{
    public:
    Renderer();
    ~Renderer();
    void init(bool* hasError);
    void update();
    void setClearColor(float r, float g, float b);
    bool isInitialized();
    private:
    bool initalized = false;
};