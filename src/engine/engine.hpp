#pragma once
#include "../vm/vm.hpp"
#include "../window/window.hpp"
#include "../renderer/renderer.hpp"
#include <stack>

class Engine{
    public:
    VM vm;
    Window window;
    Renderer renderer;
    bool running = true;
    Engine();
    ~Engine();
    void init();
    void loop();
};

extern Engine engine;