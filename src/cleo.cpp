#include <iostream>
#include "engine/engine.hpp"
#include "utils/cli.hpp"
#include "utils/fs.hpp"

#define MAJOR 0
#define MINOR 0
#define PATCH 0

int main(int argc, const char** argv){
    Cli cli(argc, argv);
    cli.next();
    if(cli.match("-h") || cli.match("--help")){
        std::cout << "Help text" << std::endl;
        return 0;
    }
    if(cli.match("-v") || cli.match("--version")){
        std::cout << "Cleo version " << MAJOR << "." << MINOR << "." << PATCH << std::endl;
        return 0;
    }
    const char* path = "./main.js";
    if(cli.hasArgs()) path = cli.next().c_str();
    engine.init(path);
    return 0;
}
