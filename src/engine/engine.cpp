#include "engine.hpp"
#include <iostream>
#include "utils/fs.hpp"
#include <fstream>

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
    auto src = readFile(&hasError, "./main.js");
    vm.bind(&hasError, src);
    window.init(&hasError);
    renderer.init(&hasError);
    audio.init(&hasError);
    if(hasError) return;
    vm.launch(&hasError);
    if(!hasError) loop();
    else running = false;
}
void Engine::loop(){
    while(running){
        renderer.update();
        vm.update(0.16);
        window.update();
        if(window.shouldClose()) running = false;
    }
}

Engine engine;