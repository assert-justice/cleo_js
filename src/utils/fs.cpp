#include "fs.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <stack>
#include "path.hpp"

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

std::string getSavePath(std::string appname){
    std::string res;
    // if windows, use %APPDATA%\cleo\appname
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    std::string appdata = std::getenv("APPDATA");
    res = appdata + "\\cleo\\" + appname;
    // else if linux use ~/.cleo/appname
    #elif __linux__
    res = "~/.cleo/" + appname;
    #else
    # error "Unknown Compiler Target"
    #endif
    createDirectory(res);
    return res;
}

char getPathSeparator(){
    // if windows, use "\"
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    return '\\';
    // else if use /
    #else
    return '/';
    #endif
}

std::string getExtension(std::string path){
    // TODO: make more robust
    std::string fname = path;
    auto idx = fname.find_last_of('.');
    if(idx == std::string::npos) return "";
    return fname.substr(idx+1);
}

bool createDirectory(std::string path){
    // Non destructively creates nested directories.
    Path p(path);
    if(p.isDirectory()) return true;
    std::stack<std::string> stack;
    while (p.size() > 0)
    {
        stack.push(p.pop());
        if(p.isDirectory()) break;
    }
    if(p.size() == 0) return false;
    while(stack.size() > 0){
        p.push(stack.top());
        stack.pop();
        // TODO: can throw exceptions, should wrap in try/catch
        bool succeeded = std::filesystem::create_directory(p.str());
        if(!succeeded) return false;
    }
    return true;
}