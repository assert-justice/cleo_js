#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>

class Window{
    public:
    GLFWwindow* window = NULL;
    int width = 800;
    int height = 600;
    int mode = 0; // 0: windowed, 1: borderless fullscreen, 2: fullscreen
    bool vsync = true;
    std::string name = "cleo";
    Window();
    ~Window();
    void init(bool* hasError);
    void update();
    bool shouldClose();
    void quit();
    void setVsync(bool vsync);
    void setStats(std::string name, int width, int height, int mode);
    bool initialized = false;
    private:
    void setWindow();
};