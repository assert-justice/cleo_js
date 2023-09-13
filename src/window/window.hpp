#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class Window{
    public:
    GLFWwindow* window = NULL;
    Window();
    ~Window();
    void init(bool* hasError);
    void update();
    bool shouldClose();
    void quit();
    private:
    bool initialized = false;
};