#include "bind_game.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

JSValue initBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    engine.vm.initFn = JS_DupValue(ctx, fn);
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

JSValue updateBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    engine.vm.updateFn = JS_DupValue(ctx, fn);
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

void bindGame(bool* hasError){
    JSValue proto;
    JSValue fn;

    proto = JS_NewObject(engine.vm.context);
    // init
    fn = JS_NewCFunction(engine.vm.context, &initBind, "init", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "init"), 
        JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    JS_FreeValue(engine.vm.context, fn);
    // update
    fn = JS_NewCFunction(engine.vm.context, &updateBind, "update", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "update"), 
        JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    JS_FreeValue(engine.vm.context, fn);
    engine.vm.addExport("Game", proto);
}