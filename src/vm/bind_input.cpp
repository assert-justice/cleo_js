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

static bool joyValid(JSContext* ctx, int joyIdx){
    if(!glfwJoystickPresent(joyIdx)){
        JS_ThrowReferenceError(ctx, "invalid joystick id!");
        return false;
    }
    if(!glfwJoystickIsGamepad(joyIdx)){
        JS_ThrowReferenceError(ctx, "joystick is not a gamepad!");
        return false;
    }
    return true;
}

static JSValue keyIsDownBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int keyCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetKey(engine.window.window, keyCode);
    return JS_NewBool(ctx, state);
}

static JSValue mouseButtonIsDownBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int mouseButtonCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    bool state = glfwGetMouseButton(engine.window.window, mouseButtonCode);
    return JS_NewBool(ctx, state);
}

static JSValue mouseGetXBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    double xPos, yPos;
    glfwGetCursorPos(engine.window.window, &xPos, &yPos);
    return JS_NewFloat64(ctx, xPos);
}

static JSValue mouseGetYBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    double xPos, yPos;
    glfwGetCursorPos(engine.window.window, &xPos, &yPos);
    return JS_NewFloat64(ctx, yPos);
}

static JSValue joyButtonIsDownBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int joyIdx = help.getInt();
    int buttonCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    if(!joyValid(ctx, joyIdx)) return JS_NewBool(ctx, false);
    if(buttonCode < 0 || buttonCode > 14){
        JS_ThrowReferenceError(ctx, "invalid joyButton code!");
        return JS_EXCEPTION;
    }
    GLFWgamepadstate state;
    glfwGetGamepadState(joyIdx, &state);
    return JS_NewBool(ctx, state.buttons[buttonCode]);
}
static JSValue joyGetAxisBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int joyIdx = help.getInt();
    int axisCode = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    if(!joyValid(ctx, joyIdx)) return JS_NewFloat64(ctx, 0);
    if(axisCode < 0 || axisCode > 5){
        JS_ThrowReferenceError(ctx, "invalid joyAxis code!");
        return JS_EXCEPTION;
    }
    GLFWgamepadstate state;
    glfwGetGamepadState(joyIdx, &state);
    return JS_NewFloat64(ctx, state.axes[axisCode]);
}
static JSValue gamepadExistsBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!inputInitialized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    int joyIdx = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    return JS_NewBool(ctx, joyValid(ctx, joyIdx));
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
    // joyButtonIsDown(joyIdx, buttonCode: number): bool
    fn = JS_NewCFunction(ctx, &joyButtonIsDownBind, "joyButtonIsDown", 0);
    JS_DefinePropertyValueStr(ctx, proto, "joyButtonIsDown", fn, 0);
    // joyGetAxis(joyIdx, axisCode: number): bool
    fn = JS_NewCFunction(ctx, &joyGetAxisBind, "joyGetAxis", 0);
    JS_DefinePropertyValueStr(ctx, proto, "joyGetAxis", fn, 0);
    // gamepadExists(joyIdx): bool
    fn = JS_NewCFunction(ctx, &gamepadExistsBind, "gamepadExists", 0);
    JS_DefinePropertyValueStr(ctx, proto, "gamepadExists", fn, 0);
    engine.vm.addExport("Input", proto);
}