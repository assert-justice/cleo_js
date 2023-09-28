#include "audio.hpp"
#include <iostream>
#include "engine/engine.hpp"

Audio::~Audio(){
    if(!initalized) return;
    ma_engine_uninit(&audioEngine);
}

void Audio::init(bool* hasError){
    if(*hasError) return;
    auto result = ma_engine_init(NULL, &audioEngine);
    if (result != MA_SUCCESS) {
        *hasError = true;
        return;  // Failed to initialize the engine.
    }
    int idx = loadSound("Jump__001.wav");
    playSound(idx);
    initalized = true;
}

int Audio::loadSound(const char* path){
    ma_result result;
    auto sound = new ma_sound();
    result = ma_sound_init_from_file(&audioEngine, path, 
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_STREAM, NULL, NULL, 
        sound);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to load sound'" << "'\n";
        return -1;
    }
    return soundStore.add(sound);
}

void Audio::playSound(int idx){
    auto sound = soundStore.get(idx);
    if(sound == nullptr){
        // TODO: actually handle this error
        return;
    }
    ma_sound_start(sound);
}