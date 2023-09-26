#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class Window{
    public:
    GLFWwindow* window = NULL;
    float width = 800;
    float height = 600;
    int mode = 0; // 0: windowed, 1: borderless fullscreen, 2: fullscreen
    Window();
    ~Window();
    void init(bool* hasError);
    void update();
    bool shouldClose();
    void quit();
    bool initialized = false;
    private:
};