#include "window.hpp"
#include <iostream>
#include "engine/engine.hpp"

static void framebufferCallback(GLFWwindow* window, int width, int height){
    window = window;
    glViewport(0, 0, width, height);
    engine.window.width = width;
    engine.window.height = height;
    // engine.renderer.setCameraPosition(0,0);
    engine.renderer.setDimensions(width, height);
}

// needs to be attached with glfwSetKeyCallback *after* the window is initialized
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(JS_IsUndefined(engine.vm.keyCallbackFn)) return;
    std::string message = "";
    if(action == GLFW_PRESS){message = "press";}
    else if(action == GLFW_RELEASE){message = "release";}
    else{
        // GLFW_REPEAT currently not supported
        return;
    }
    auto keyCode = JS_NewInt32(engine.vm.context, key);
    auto actionMessage = JS_NewString(engine.vm.context, message.c_str());
    JSValue arr[] = {keyCode, actionMessage};
    auto res = JS_Call(engine.vm.context, engine.vm.keyCallbackFn, JS_UNDEFINED, 2, arr);
    engine.vm.handleIfException(engine.vm.context, res);
}

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
    glfwSetKeyCallback(window, keyCallback);
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
    int refreshRate = GLFW_DONT_CARE;
    if(mode != 0) monitor = glfwGetPrimaryMonitor();
    if(mode == 1){
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
 
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        width = mode->width;
        height = mode->height;
        refreshRate = mode->refreshRate;
    }
    if(window == NULL){
        window = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);
    }
    else{
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, refreshRate);
    }
}