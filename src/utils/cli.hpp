#pragma once
#include <string>
#include <vector>

class Cli{
    public:
    Cli(int argc, const char** argv);
    std::string next();
    std::string peek();
    bool hasArgs();
    bool match(std::string str);
    bool hasError();
    private:
    int argc;
    const char** argv;
    std::string errMessage = "";
    int idx = 0;
};