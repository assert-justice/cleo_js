#include "cli.hpp"

Cli::Cli(int argc_, const char** argv_){
    argc = argc_; argv = argv_;
}

std::string Cli::next(){
    if(hasError()) return "";
    std::string res = peek();
    if(hasError()) return "";
    idx++;
    return res;
}
std::string Cli::peek(){
    if(hasError()) return "";
    if(!hasArgs()){
        errMessage = "Out of arguments!";
        return "";
    }
    return argv[idx];
}

bool Cli::match(std::string str){
    bool res = peek() == str;
    if(hasError()) return false;
    return res;
}

bool Cli::hasArgs(){
    return idx < argc;
}

bool Cli::hasError(){
    return errMessage.length() > 0;
}