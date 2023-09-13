#include "window.hpp"
#include <iostream>

Window::Window(){}
Window::~Window(){
    if(!initialized) return;
    glfwTerminate();
}

void Window::init(bool* hasError){
    if(*hasError) return;
    // GLFWwindow* window;
    if(!glfwInit()){
        std::cout << "Failed to init glfw" << std::endl;
        *hasError = true;
        return;
    }
    GLFWwindow* window = glfwCreateWindow(800, 600, "hi glfw", NULL, NULL);
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
    // enable vsync
    glfwSwapInterval(1);
    this->window = window;
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