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
    void init(const char* path);
    void loop();
    void setError();
    private:
    bool running = true;
    bool hasError = false;
    std::string errorMessage;
};

extern Engine engine;