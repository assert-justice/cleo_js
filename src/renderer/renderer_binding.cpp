#include "renderer_binding.hpp"
#include "quickjs.h"
#include "engine/engine.hpp"
#include <iostream>

JSModuleDef* renderMod;

void greet(std::string message){
    std::cout << "hi " << message << std::endl;
}

JSValue greetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto message = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    greet(message);
    return JS_UNDEFINED;
}

JSValue setClearColorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto r = help.getFloat64();
    auto g = help.getFloat64();
    auto b = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    // engine.renderer.setClearColor(r, g, b);
    std::cout << "set color to " << r << g << b << std::endl;
    return JS_UNDEFINED;
}

int setRenderMod(JSContext* ctx, JSModuleDef* mod){
    std::cout << "init render module" << std::endl;
    std::string name = "hello";
    auto fn = JS_NewCFunction(engine.vm.context, &greetBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, renderMod, name.c_str(), fn);
    name = "setClearColor";
    fn = JS_NewCFunction(engine.vm.context, &setClearColorBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, renderMod, name.c_str(), fn);
    return 0;
}

void bindRenderer(bool* hasError){
    if(*hasError) return;
    renderMod = JS_NewCModule(engine.vm.context, "cleo-renderer", &setRenderMod);
    // std::string name = "hello";
    JS_AddModuleExport(engine.vm.context, renderMod, "hello");
    // name = "setClearColor";
    JS_AddModuleExport(engine.vm.context, renderMod, "setClearColor");
    // std::string src = "import {setClearColor} from 'cleo-renderer'; setClearColor(0,0,0);";
    // auto val = JS_Eval(engine.vm.context, src.c_str(), src.size(), "temp", JS_EVAL_TYPE_MODULE);
    // if(!isException(engine.vm.context, val)) JS_FreeValue(engine.vm.context, val);
}