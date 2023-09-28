#include "window.hpp"
#include <iostream>
#include "engine/engine.hpp"

static void framebufferCallback(GLFWwindow* window, int width, int height){
    window = window;
    glViewport(0, 0, width, height);
    engine.window.width = width;
    engine.window.height = height;
    engine.renderer.setCameraPosition(0,0);
}

Window::Window(){}
Window::~Window(){
    if(!initialized) return;
    initialized = false;
    glfwTerminate();
}

void Window::init(bool* hasError){
    if(*hasError) return;
    if(!glfwInit()){
        std::cout << "Failed to init glfw" << std::endl;
        *hasError = true;
        return;
    }
    setWindow();
    // window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        *hasError = true;
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        *hasError = true;
        return;
    }
    glfwSetFramebufferSizeCallback(window, framebufferCallback);
    glfwSwapInterval(vsync);
    initialized = true;
}

void Window::update(){
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(window);
}

void Window::quit(){
    glfwSetWindowShouldClose(window, true);
}

void Window::setStats(std::string name, int width, int height, int mode){
    this->name = name; 
    this->width = width; this->height = height; 
    this->mode = mode;
    // if the window isn't yet initialized we're done
    if(!initialized) return;
    // regenerate the window
    setWindow();
}

void Window::setVsync(bool vsync){
    this->vsync = vsync;
    if(!initialized) return;
    glfwSwapInterval(vsync);
}

void Window::setWindow(){
    GLFWmonitor* monitor = NULL;
    if(window == NULL){
        window = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);
    }
    else{
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, GLFW_DONT_CARE);
    }
}