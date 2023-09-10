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