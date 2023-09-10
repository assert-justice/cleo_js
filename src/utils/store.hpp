#pragma once
#include <unordered_map>

template <typename T>
class Store{
    public:
    ~Store(){
        for(auto it=data.begin();it!=this->data.end();it++) {
        delete it->second; 
    }
    }
    int add(T* ptr){
        data.insert(std::make_pair(idCount, ptr));
        idCount++;
        return idCount - 1;
    }
    T* get(int id){
        return data.at(id);
    }
    bool has(int id){
        return data.count(id) == 1;
    }
    private:
    std::unordered_map<int, T*> data;
    int idCount = 0;
};