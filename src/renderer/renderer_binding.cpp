#include "renderer_binding.hpp"
#include "quickjs.h"
#include "engine/engine.hpp"
#include <iostream>

// void setClearColorBind(){
    // engine.renderer.setClearColor()
// }

JSModuleDef* renderMod;

void greet(const char* message){
    std::cout << "hi " << message << std::endl;
}

JSValue greetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(argc > 0){
        auto str = JS_ToCString(engine.vm.context, argv[0]);
        greet(str);
        JS_FreeCString(engine.vm.context, str);
    }
    // greet();
    return JS_UNDEFINED;
}

// JSModuleDef* renderMod;
int setRenderMod(JSContext* ctx, JSModuleDef* mod){
    std::cout << "init render module" << std::endl;
    std::string name = "hello";
    auto fn = JS_NewCFunction(engine.vm.context, &greetBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, renderMod, name.c_str(), fn);
    return 0;
}

void bindRenderer(bool* hasError){
    if(*hasError) return;
    // JSModuleInitFunc
    // std::cout << "runtime: " << engine.vm.runtime << std::endl;
    // std::cout << "context: " << engine.vm.context << std::endl;
    renderMod = JS_NewCModule(engine.vm.context, "cleo-renderer", &setRenderMod);
    // JSCFunction
    std::string name = "hello";
    JS_AddModuleExport(engine.vm.context, renderMod, name.c_str());
    std::string src = "import {hello} from 'cleo-renderer'; hello('there');";
    JS_Eval(engine.vm.context, src.c_str(), src.size(), "temp", JS_EVAL_TYPE_MODULE);
}