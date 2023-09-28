#pragma once
#include "vm/vm.hpp"
#include "window/window.hpp"
#include "renderer/renderer.hpp"
#include "audio/audio.hpp"
#include <stack>

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
};

extern Engine engine;