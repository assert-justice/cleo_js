#include "bind_window.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

static JSValue windowInitalized(JSContext* ctx){
    if(!engine.window.initialized){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

JSValue getWidthBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(windowInitalized(ctx))) return JS_EXCEPTION;
    auto val = JS_NewFloat64(ctx, engine.window.width);
    return val;
}
JSValue getHeightBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(windowInitalized(ctx))) return JS_EXCEPTION;
    auto val = JS_NewFloat64(ctx, engine.window.height);
    return val;
}

void bindWindow(){
    JSValue proto;
    JSValue fn;

    proto = JS_NewObject(engine.vm.context);
    // width
    fn = JS_NewCFunction(engine.vm.context, &getWidthBind, "width", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "width"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(engine.vm.context, fn);
    fn = JS_NewCFunction(engine.vm.context, &getHeightBind, "height", 0);
    JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "height"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(engine.vm.context, fn);
    // update
    // fn = JS_NewCFunction(engine.vm.context, &updateBind, "update", 0);
    // JS_DefineProperty(engine.vm.context, proto, JS_NewAtom(engine.vm.context, "update"), 
    //     JS_UNDEFINED, JS_UNDEFINED, fn, JS_PROP_HAS_SET);
    // JS_FreeValue(engine.vm.context, fn);
    engine.vm.addExport("Window", proto);
}