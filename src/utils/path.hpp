#pragma once
#include <string>
#include <vector>

class Path{
    public:
    Path(std::string str);
    // Not yet implemented
    // Path(char*);
    // Path(std::string);
    std::string str();
    std::string getExtension();
    bool exists();
    bool isFile();
    bool isDirectory();
    std::string pop();
    void push(std::string str);
    size_t size();
    private:
    std::vector<std::string> data;
    void splitPathString(std::string str);
};
