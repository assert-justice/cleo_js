#pragma once
#include "miniaudio.h"
#include "utils/store.hpp"

class Audio{
    public:
    // Audio();
    ~Audio();
    void init(bool* hasError);
    int newGroup();
    void freeGroup(int groupId);
    int soundLoad(const char* path, bool streamingEnabled, int groupId);
    void soundFree(int idx);
    void soundPlay(int idx);
    void soundPause(int idx);
    void soundStop(int idx);
    bool soundIsPlaying(int idx);
    void soundSetVolume(int idx, float volume);
    float soundGetVolume(int idx);
    void soundLoop(int idx, bool shouldLoop);
    bool soundIsLooping(int idx);
    bool initalized = false;
    ma_engine audioEngine;
    Store<ma_sound> soundStore;
    Store<ma_sound_group> groupStore;
    private:
    bool validIdx(int idx);
};