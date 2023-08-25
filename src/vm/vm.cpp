#include "vm.hpp"
#include <iostream>

VM::VM(){
    //
}

VM::~VM(){
    // std::cout << "uninit vm" << std::endl;
    if(!initialized) return;
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);
}

void VM::init(bool* hasError){
    if(*hasError) return;
    runtime = JS_NewRuntime();
    context = JS_NewContext(runtime);
    initialized = true;
}