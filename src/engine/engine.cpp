#include "engine.hpp"
#include <iostream>
#include "renderer/renderer_binding.hpp"

Engine::Engine(){
    // bool hasError = false;
    // // vm.init(&hasError);
    // window.init(&hasError);
    // renderer.init(&hasError);
    // if(!hasError) loop();
    // else running = false;
}
Engine::~Engine(){
    //
}
void Engine::init(){
    bool hasError = false;
    vm.init(&hasError);
    bindRenderer(&hasError);
    window.init(&hasError);
    renderer.init(&hasError);
    if(!hasError) loop();
    else running = false;
}
void Engine::loop(){
    while(running){
        renderer.update();
        window.update();
        if(window.shouldClose()) running = false;
    }
}

Engine engine;