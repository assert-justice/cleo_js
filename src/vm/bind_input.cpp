#include "bind_input.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include <iostream>

static bool inputInitialized(JSContext* ctx){
    if(!engine.window.initialized){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return false;
    }
    return true;
}

JSValue keyIsDownBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int keyCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetKey(engine.window.window, keyCode);
    return JS_NewBool(ctx, state);
}

JSValue mouseButtonIsDownBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int mouseButtonCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetMouseButton(engine.window.window, mouseButtonCode);
    return JS_NewBool(ctx, state);
}

JSValue mouseGetXBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    double xPos, yPos;
    glfwGetCursorPos(engine.window.window, &xPos, &yPos);
    return JS_NewFloat64(ctx, xPos);
}

JSValue mouseGetYBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    double xPos, yPos;
    glfwGetCursorPos(engine.window.window, &xPos, &yPos);
    return JS_NewFloat64(ctx, yPos);
}

void bindInput(){
    JSValue proto;
    JSValue fn;
    JSContext* ctx = engine.vm.context;
    proto = JS_NewObject(ctx);
    // keyIsDown(keyCode: number): bool
    fn = JS_NewCFunction(ctx, &keyIsDownBind, "keyIsDown", 0);
    JS_DefinePropertyValueStr(ctx, proto, "keyIsDown", fn, 0);
    // mouseButtonIsDown(mouseButtonCode: number): bool
    fn = JS_NewCFunction(ctx, &mouseButtonIsDownBind, "mouseButtonIsDown", 0);
    JS_DefinePropertyValueStr(ctx, proto, "mouseButtonIsDown", fn, 0);
    // get mouseX: number
    fn = JS_NewCFunction(ctx, &mouseGetXBind, "mouseX", 0);
    JS_DefineProperty(ctx, proto, JS_NewAtom(ctx, "mouseX"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // get mouseY: number
    fn = JS_NewCFunction(ctx, &mouseGetYBind, "mouseY", 0);
    JS_DefineProperty(ctx, proto, JS_NewAtom(ctx, "mouseY"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    engine.vm.addExport("Input", proto);
}