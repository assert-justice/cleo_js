#include "bind_system.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

static JSValue printlnBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto str = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    std::cout << str << std::endl;
    return JS_UNDEFINED;
}

void bindSystem(bool* hasError){
    JSValue proto;
    JSValue fn;
    proto = JS_NewObject(engine.vm.context);
    // println
    fn = JS_NewCFunction(engine.vm.context, &printlnBind, "println", 0);
    // fn = JS_NewFloat64(engine.vm.context, 0.0);
    JS_DefinePropertyValue(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "println"), 
        fn, 0);
    // JS_FreeValue(engine.vm.context, fn);
    engine.vm.addExport("System", proto);
}