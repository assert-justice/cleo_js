#include "bind_input.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

JSValue getKeyBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.window.initialized){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    int keyCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetKey(engine.window.window, keyCode);
    return JS_NewBool(ctx, state);
}

void bindInput(bool* hasError){
    JSValue proto;
    JSValue fn;
    proto = JS_NewObject(engine.vm.context);
    // println
    fn = JS_NewCFunction(engine.vm.context, &getKeyBind, "getKey", 0);
    // fn = JS_NewFloat64(engine.vm.context, 0.0);
    JS_DefinePropertyValue(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "getKey"), 
        fn, 0);
    // JS_FreeValue(engine.vm.context, fn);
    engine.vm.addExport("Input", proto);
}