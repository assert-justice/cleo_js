#include "fs.hpp"
#include <fstream>
#include <iostream>

std::string readFile(bool* hasError, const char* path){
    if(*hasError) return "";
    std::ifstream file(path);
    if(file.is_open()){
        std::string res(std::istreambuf_iterator<char>{file}, {});
        file.close();
        return res;
    }
    else{
        std::cerr << "unable to open file at '" << path << "'!" << std::endl;
        *hasError = true;
        return "";
    }
}

std::string getExtension(const char* path){
    // TODO: make more robust
    std::string fname = path;
    auto idx = fname.find_last_of('.');
    if(idx == std::string::npos) return "";
    return fname.substr(idx+1);
}