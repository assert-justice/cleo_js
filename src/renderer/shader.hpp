#pragma once
#include <string>

class Shader{
    public:
    unsigned int id;
    bool initialized = false;
    void initialize(bool* hasError, std::string vertexSrc, std::string fragmentSrc);
    // bool hasError = false;
    // Shader(std::string vertexSrc, std::string fragmentSrc);
    ~Shader();
    void use();
    unsigned int getUniformLocation(const char* name);
    unsigned int getAttribLocation(const char* name);
};