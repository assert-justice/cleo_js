#include "engine.hpp"
#include <iostream>
#include "renderer/renderer_binding.hpp"
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
    bindRenderer(&hasError);
    // std::ifstream file("./main.js");
    // if(file.is_open()){
    //     std::string src(std::istreambuf_iterator<char>{file}, {});
    // }
    // else{
    //     std::cerr << "unable to open 'main.js'" << std::endl;
    //     hasError = true;
    // }
    auto src = readFile(&hasError, "./main.js");
    vm.bind(&hasError, src);
    window.init(&hasError);
    renderer.init(&hasError);
    if(hasError) return;
    vm.launch();
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