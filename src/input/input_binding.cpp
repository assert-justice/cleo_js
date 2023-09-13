#include "input_binding.hpp"
#include "quickjs.h"
#include "engine/engine.hpp"
#include "glfw/glfw3.h"
#include <iostream>

JSValue setGetKeyBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    //
    FnHelp help(ctx, argc, argv);
    int keyCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetKey(engine.window.window, keyCode);
    return JS_NewBool(ctx, state);
}

int setInputMod(JSContext* ctx, JSModuleDef* mod){
    std::string name = "getKey";
    auto fn = JS_NewCFunction(engine.vm.context, &setGetKeyBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    // name = "loadImage";
    // fn = JS_NewCFunction(engine.vm.context, &setLoadImage, name.c_str(), 0);
    // JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    // name = "drawImage";
    // fn = JS_NewCFunction(engine.vm.context, &setDrawImage, name.c_str(), 0);
    // JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    return 0;
}

void bindInput(bool* hasError){
    if(*hasError) return;
    auto renderMod = JS_NewCModule(engine.vm.context, "cleo-input", &setInputMod);
    JS_AddModuleExport(engine.vm.context, renderMod, "getKey");
}