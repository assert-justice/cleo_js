#pragma once
#include "miniaudio.h"
#include "utils/store.hpp"

class Audio{
    public:
    Audio();
    ~Audio();
    void init(bool* hasError);
    bool initalized = false;
    Store<ma_sound> soundStore;
    private:
};