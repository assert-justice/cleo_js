#pragma once
#include <unordered_map>

template <typename T>
class Store{
    public:
    ~Store(){
        // for(auto it=data.begin();it!=data.end();it++) {
        //     delete it->second; 
        // }
        clear();
    }
    int add(T* ptr){
        data.insert(std::make_pair(idCount, ptr));
        idCount++;
        return idCount - 1;
    }
    T* get(int id){
        if(!has(id)) return nullptr;
        return data.at(id);
    }
    bool has(int id){
        return data.count(id) == 1;
    }
    void del(int id){
        if(!has(id)) return;
        delete get(id);
        data.erase(id);
    }
    void clear(){
        for(auto it=data.begin();it!=data.end();it++) {
            delete it->second;
        }
        data.clear();
    }
    private:
    std::unordered_map<int, T*> data;
    int idCount = 0;
};