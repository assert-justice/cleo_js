#include "bind_engine.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

static JSValue initBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    if(help.hasError) return JS_EXCEPTION;
    engine.vm.initFn = JS_DupValue(ctx, fn);
    return JS_UNDEFINED;
}
static JSValue drawBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    if(help.hasError) return JS_EXCEPTION;
    engine.vm.drawFn = JS_DupValue(ctx, fn);
    return JS_UNDEFINED;
}

static JSValue quitBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    engine.window.quit();
    return JS_UNDEFINED;
}

static JSValue updateBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    engine.vm.updateFn = JS_DupValue(ctx, fn);
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

void bindEngine(){
    JSValue proto;
    JSValue fn;

    proto = JS_NewObject(engine.vm.context);
    // set init: ()=>void
    fn = JS_NewCFunction(engine.vm.context, &initBind, "init", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "init"), 
        JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    JS_FreeValue(engine.vm.context, fn);
    // set update: (dt: number)=>void
    fn = JS_NewCFunction(engine.vm.context, &updateBind, "update", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "update"), 
        JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    JS_FreeValue(engine.vm.context, fn);
    // set draw: ()=>void
    fn = JS_NewCFunction(engine.vm.context, &drawBind, "draw", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "draw"), 
        JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    JS_FreeValue(engine.vm.context, fn);
    // quit(): void
    fn = JS_NewCFunction(engine.vm.context, &quitBind, "quit", 0);
    JS_DefinePropertyValue(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "quit"), 
        fn, 0);
    engine.vm.addExport("Engine", proto);
}