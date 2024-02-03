#include "audio.hpp"
#include <iostream>
#include "engine/engine.hpp"

Audio::~Audio(){
    if(!initalized) return;
    initalized = false;
    ma_engine_uninit(&audioEngine);
}

void Audio::init(bool* hasError){
    if(*hasError) return;
    auto result = ma_engine_init(NULL, &audioEngine);
    if (result != MA_SUCCESS) {
        *hasError = true;
        return;  // Failed to initialize the engine.
    }
    // int idx = soundLoad("Jump__001.wav");
    // soundPlay(idx);
    initalized = true;
}

int Audio::newGroup(){
    ma_result result;
    auto group = new ma_sound_group();
    result = ma_sound_group_init(&audioEngine, 0, NULL, group);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to init sound group." << std::endl; 
        return -1;
    }
    return groupStore.add(group);
}

void Audio::freeGroup(int groupId){
    groupStore.del(groupId);
}

int Audio::soundLoad(const char* path, bool streamingEnabled, int groupId){
    ma_result result;
    ma_sound_group* group = NULL;
    if(groupId != -1){
        if(!groupStore.has(groupId)){
            std::cout << "Failed to load sound '" << path << "', invalid group." << std::endl;
            return -1;
        }
        group = groupStore.get(groupId);
    }
    auto sound = new ma_sound();
    result = ma_sound_init_from_file(&audioEngine, path, 
        MA_SOUND_FLAG_NO_SPATIALIZATION | 
        streamingEnabled, group, NULL, 
        sound);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to load sound '" << path << "'" << std::endl;
        return -1;
    }
    return soundStore.add(sound);
}

void Audio::soundFree(int idx){
    soundStore.del(idx);
}

void Audio::soundPlay(int idx){
    if(!validIdx(idx)) return;
    ma_sound_start(soundStore.get(idx));
}
void Audio::soundPause(int idx){
    if(!validIdx(idx)) return;
    ma_sound_stop(soundStore.get(idx));
}
void Audio::soundStop(int idx){
    if(!validIdx(idx)) return;
    auto sound = soundStore.get(idx);
    ma_sound_stop(sound);
    ma_sound_seek_to_pcm_frame(sound, 0);
}
bool Audio::soundIsPlaying(int idx){
    if(!validIdx(idx)) return false;
    return ma_sound_is_playing(soundStore.get(idx));
}
void Audio::soundSetVolume(int idx, float volume){
    if(!validIdx(idx)) return;
    ma_sound_set_volume(soundStore.get(idx), volume);
}

float Audio::soundGetVolume(int idx){
    if(!validIdx(idx)) return 0.0;
    return ma_sound_get_volume(soundStore.get(idx));
}

void Audio::soundLoop(int idx, bool shouldLoop){
    if(!validIdx(idx)) return;
    ma_sound_set_looping(soundStore.get(idx), (ma_bool32) shouldLoop);
}
bool Audio::soundIsLooping(int idx){
    if(!validIdx(idx)) return 0;
    return (int)ma_sound_is_looping(soundStore.get(idx));
}

bool Audio::validIdx(int idx){
    if(!soundStore.has(idx)){
        // TODO: actually handle this error
        return false;
    }
    return true;
}