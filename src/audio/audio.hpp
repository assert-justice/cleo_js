#pragma once
#include "miniaudio.h"
#include "utils/store.hpp"

class Audio{
    public:
    // Audio();
    ~Audio();
    void init(bool* hasError);
    int soundLoad(const char* path);
    void soundPlay(int idx);
    void soundPause(int idx);
    void soundStop(int idx);
    bool soundIsPlaying(int idx);
    void soundSetVolume(int idx, double volume);
    double soundGetVolume(int idx);
    void soundLoop(int idx, bool shouldLoop);
    bool soundIsLooping(int idx);
    bool initalized = false;
    ma_engine audioEngine;
    Store<ma_sound> soundStore;
    private:
    bool validIdx(int idx);
};