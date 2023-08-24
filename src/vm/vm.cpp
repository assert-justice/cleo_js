#include "vm.hpp"
#include <iostream>

VM::VM(){
    //
}

VM::~VM(){
    // std::cout << "uninit vm" << std::endl;
    if(!initialized) return;
    delete publicContext;
    delete publicRuntime;
}

void VM::init(bool* hasError){
    if(! *hasError) return;
    std::cout << "init vm" << std::endl;
    publicRuntime = new qjs::Runtime();
    publicContext = new qjs::Context(publicRuntime->rt);
    initialized = true;
}