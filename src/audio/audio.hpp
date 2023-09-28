#pragma once
#include "miniaudio.h"
#include "utils/store.hpp"

class Audio{
    public:
    // Audio();
    ~Audio();
    void init(bool* hasError);
    int loadSound(const char* path);
    void playSound(int idx);
    void pauseSound(int idx);
    void stopSound(int idx);
    int soundIsPlaying(int idx);
    void setVolumeSound(int idx, double volume);
    double getVolumeSound(int idx);
    void loopSound(int idx, int shouldLoop);
    int SoundIsLooping(int idx);
    bool initalized = false;
    ma_engine audioEngine;
    Store<ma_sound> soundStore;
    private:
};