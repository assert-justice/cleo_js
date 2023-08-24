#include "engine.hpp"
#include <iostream>

Engine::Engine(){
    bool hasError = false;
    vm.init(&hasError);
    window.init(&hasError);
    renderer.init(&hasError);
    if(!hasError) loop();
    else running = false;
}
Engine::~Engine(){
    //
}
void Engine::loop(){
    while(running){
        renderer.update();
        window.update();
        if(window.shouldClose()) running = false;
    }
}

Engine engine;