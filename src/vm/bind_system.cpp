#include <iostream>
#include <string>
#include "bind_system.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include "utils/fs.hpp"

static void print(JSContext* ctx, JSValue val){
    JSValue str;
    if(JS_IsString(val)) str = val;
    else str = JS_ToString(ctx, val);
    auto cString = JS_ToCString(ctx, str);
    std::cout << cString;
    JS_FreeCString(ctx, cString);
}

// Needed for user created cli but is blocking. Maybe shouldn't be included?
static JSValue inputBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    std::string str;
    std::cin >> str;
    return JS_NewString(ctx, str.c_str());
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

static JSValue readFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto path = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    bool hasError = false;
    auto text = readFile(&hasError, path.c_str());
    if(hasError){
        JS_ThrowReferenceError(ctx, "unable to read file at '%s'!", path.c_str());
        return JS_EXCEPTION;
    }
    return JS_NewString(ctx, text.c_str());
}

static JSValue writeFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto path = fnHelp.getString();
    auto text = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    bool hasError = false;
    writeFile(&hasError, path.c_str(), text);
    if(hasError){
        JS_ThrowReferenceError(ctx, "unable to write to file at '%s'!", path.c_str());
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

void bindSystem(){
    JSValue proto;
    JSValue fn;
    proto = JS_NewObject(engine.vm.context);
    // println(...any[]): void
    fn = JS_NewCFunction(engine.vm.context, &printlnBind, "println", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "println", fn, 0);
    // input(): string
    fn = JS_NewCFunction(engine.vm.context, &inputBind, "input", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "input", fn, 0);
    // readFile(path: string): string
    fn = JS_NewCFunction(engine.vm.context, &readFileBind, "readFile", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "readFile", fn, 0);
    // writeFile(path: string): string
    fn = JS_NewCFunction(engine.vm.context, &writeFileBind, "writeFile", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "writeFile", fn, 0);
    engine.vm.addExport("System", proto);
}