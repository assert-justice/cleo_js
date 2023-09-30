#pragma once
#include "vm/vm.hpp"
#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "audio/audio.hpp"
#include <string>

class Engine{
    public:
    VM vm;
    Window window;
    Renderer renderer;
    Audio audio;
    bool running = true;
    Engine();
    ~Engine();
    void init();
    void loop();
    void setError();
    private:
    bool hasError = false;
    std::string errorMessage;
};

extern Engine engine;