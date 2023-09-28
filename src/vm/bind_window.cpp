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
    auto val = JS_NewFloat64(ctx, (float)engine.window.width);
    return val;
}
JSValue getHeightBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(windowInitalized(ctx))) return JS_EXCEPTION;
    auto val = JS_NewFloat64(ctx, (float)engine.window.height);
    return val;
}
static int getMode(std::string mode){
    if(mode == "windowed") return 0;
    else if(mode == "borderless") return 1;
    else if(mode == "fullscreen") return 2;
    else return -1;
}

JSValue setStatsBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    // it is ok to use this function before the window is initalized
    auto name = fnHelp.getString();
    auto width = (int)fnHelp.getFloat64();
    auto height = (int)fnHelp.getFloat64();
    std::string modeStr = fnHelp.hasArgs() ? fnHelp.getString() : "windowed";
    auto mode = getMode(modeStr);
    if(mode == -1){
        JS_ThrowTypeError(ctx, "unexpected mode value '%s'!", modeStr.c_str());
        return JS_EXCEPTION;
    }
    engine.window.setStats(name, width, height, mode);
    return JS_UNDEFINED;
}

void bindWindow(){
    JSValue proto;
    JSValue fn;
    auto ctx = engine.vm.context;

    proto = JS_NewObject(ctx);
    // get width: number
    fn = JS_NewCFunction(ctx, &getWidthBind, "width", 0);
    JS_DefineProperty(ctx, proto, JS_NewAtom(ctx, "width"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // get height: number
    fn = JS_NewCFunction(ctx, &getHeightBind, "height", 0);
    JS_DefineProperty(ctx, proto, JS_NewAtom(ctx, "height"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // set(name: string, width: number, height: number, mode: string, vsync: bool)
    fn = JS_NewCFunction(ctx, &setStatsBind, "setStats", 0);
    JS_DefinePropertyValueStr(ctx, proto, "setStats", fn, 0);
    engine.vm.addExport("Window", proto);
}