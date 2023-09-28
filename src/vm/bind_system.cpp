#include "bind_system.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

static void print(JSContext* ctx, JSValue val){
    JSValue str;
    if(JS_IsString(val)) str = val;
    else str = JS_ToString(ctx, val);
    auto cString = JS_ToCString(ctx, str);
    std::cout << cString;
    JS_FreeCString(ctx, cString);
}

static JSValue printlnBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(argc > 0) print(ctx, argv[0]);
    for(int i = 1; i < argc; i++){
        std::cout << " ";
        print(ctx, argv[i]);
    }
    std::cout << std::endl;
    return JS_UNDEFINED;
}

void bindSystem(){
    JSValue proto;
    JSValue fn;
    proto = JS_NewObject(engine.vm.context);
    // println()
    fn = JS_NewCFunction(engine.vm.context, &printlnBind, "println", 0);
    JS_DefinePropertyValue(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "println"), 
        fn, 0);
    engine.vm.addExport("System", proto);
}