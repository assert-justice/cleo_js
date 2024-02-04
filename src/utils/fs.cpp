#include "fs.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

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

void writeFile(bool* hasError, const char* path, std::string text){
    if(*hasError) return;
    std::ofstream file(path);
    if(file.is_open()){
        file << text;
        file.close();
    }
    else{
        std::cerr << "unable to open file at '" << path << "'!" << std::endl;
        *hasError = true;
        return;
    }
}

std::string getExtension(const char* path){
    // TODO: make more robust
    std::string fname = path;
    auto idx = fname.find_last_of('.');
    if(idx == std::string::npos) return "";
    return fname.substr(idx+1);
}

std::string getSavePath(std::string appname){
    // if windows, use %APPDATA%\cleo\appname
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::string appdata = std::getenv("APPDATA");
    return appdata + "\\cleo\\" + appname;
    // else if linux use ~/.cleo/appname
    #elif __linux__
    return "~/.cleo/" + appname;
    #else
    # error "Unknown Compiler Target"
    #endif
}