#include "path.hpp"
#include <sstream>
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include "fs.hpp"

Path::Path(std::string str){
    splitPathString(str);
}
// Path::Path(char*){}
// Path::Path(std::string){}

std::string Path::str(){
    std::stringstream stream;
    char sep = getPathSeparator();
    stream << data[0];
    for (size_t idx = 1; idx < data.size(); idx++)
    {
        stream << sep << data[idx];
    }
    return stream.str();
}

std::string Path::getExtension(){
    // TODO: make more robust
    std::string fname = data[data.size()-1];
    auto idx = fname.find_last_of('.');
    if(idx == std::string::npos) return "";
    return fname.substr(idx+1);
}

bool Path::exists(){
    return std::filesystem::exists(str());
}
bool Path::isFile(){
    return std::filesystem::is_regular_file(str());
}
bool Path::isDirectory(){
    return std::filesystem::is_directory(str());
}
size_t Path::size(){
    return data.size();
}
void Path::push(std::string str){
    return data.push_back(str);
}
std::string Path::pop(){
    auto res = data[data.size()-1];
    data.pop_back();
    return res;
}

void Path::splitPathString(std::string str){
    std::string pathStr = std::filesystem::absolute(str).string();
    char sep = getPathSeparator();
    size_t start = 0;
    data.clear();
    while (true)
    {
        auto idx = pathStr.find(sep, start);
        if(idx == std::string::npos){
            data.push_back(pathStr.substr(start));
            break;
        }
        data.push_back(pathStr.substr(start, idx-start));
        start = idx + 1;
    }
}