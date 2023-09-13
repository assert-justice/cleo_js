#include "window_binding.hpp"
#include "quickjs.h"
#include "engine/engine.hpp"
#include "glfw/glfw3.h"
#include <iostream>

JSValue setQuitBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    engine.window.quit();
    return JS_UNDEFINED;
}

int setWindowMod(JSContext* ctx, JSModuleDef* mod){
    std::string name = "quit";
    auto fn = JS_NewCFunction(engine.vm.context, &setQuitBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    // name = "loadImage";
    // fn = JS_NewCFunction(engine.vm.context, &setLoadImage, name.c_str(), 0);
    // JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    // name = "drawImage";
    // fn = JS_NewCFunction(engine.vm.context, &setDrawImage, name.c_str(), 0);
    // JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    return 0;
}

void bindWindow(bool* hasError){
    if(*hasError) return;
    auto windowMod = JS_NewCModule(engine.vm.context, "cleo-window", &setWindowMod);
    JS_AddModuleExport(engine.vm.context, windowMod, "quit");
}