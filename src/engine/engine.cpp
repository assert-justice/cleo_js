#include "engine.hpp"
#include <iostream>
#include "utils/fs.hpp"
#include <fstream>
#include "sys/time.h"

static double getTime(){
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return (double)current_time.tv_sec + (double)current_time.tv_usec / 1000000; 
}

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
    audio.init(&hasError);
    window.init(&hasError);
    renderer.init(&hasError);
    if(hasError) return;
    vm.launch(&hasError);
    if(!hasError) loop();
    else running = false;
}
void Engine::loop(){
    double t = 0.0;
    double tickRate = 60; // ticks / second
    const double dt = 1.0 / tickRate; // target delta time
    double currentTime = getTime();
    double acc = 0.0;
    double scriptTime;
    double elapsedTime = 0;
    while (running){
        double newTime = getTime();
        // time since last frame
        // the frameTime is our budget for game logic
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        // we add the frame time to the accumulator 
        // in this way "left over" time will roll over to the next render loop
        acc += frameTime;
        // while the time budget has at least one more full time for a script update
        while(acc >= dt){
            scriptTime = getTime();
            vm.update(dt);
            elapsedTime = getTime() - scriptTime;
            acc -= dt;
            t += dt;
        }
        renderer.update();
        renderer.enabled = true;
        vm.draw();
        renderer.enabled = false;
        window.update();
        if(window.shouldClose()) running = false;
    }
}

Engine engine;