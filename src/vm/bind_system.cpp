#include <iostream>
#include <string>
#include <sstream>
#include "bind_system.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include "utils/fs.hpp"
#include "utils/path.hpp"

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

static JSValue getSavePathBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto appname = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    auto path = getSavePath(appname);
    auto res = JS_NewString(ctx, path.c_str());
    return res;
}

static JSValue joinPathBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto separator = getPathSeparator();
    std::stringstream stream;
    stream << fnHelp.getString();
    while(!fnHelp.hasError && fnHelp.hasArgs()){
        stream << separator << fnHelp.getString();
    }
    if(fnHelp.hasError) return JS_EXCEPTION;
    auto res = JS_NewString(ctx, stream.str().c_str());
    return res;
}

static JSValue createDirectoryBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto path = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    return JS_NewBool(ctx, createDirectory(path));
}
static JSValue isPathBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto pathStr = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    return JS_NewBool(ctx, Path(pathStr).exists());
}
static JSValue isFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto pathStr = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    return JS_NewBool(ctx, Path(pathStr).isFile());
}
static JSValue isDirectoryBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp fnHelp(ctx, argc, argv);
    auto pathStr = fnHelp.getString();
    if(fnHelp.hasError) return JS_EXCEPTION;
    return JS_NewBool(ctx, Path(pathStr).isDirectory());
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
    // getSavePath(appname: string): string
    fn = JS_NewCFunction(engine.vm.context, &getSavePathBind, "getSavePath", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "getSavePath", fn, 0);
    // joinPath(...string[]): string
    fn = JS_NewCFunction(engine.vm.context, &joinPathBind, "joinPath", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "joinPath", fn, 0);
    // createDirectory(...string[]): bool
    fn = JS_NewCFunction(engine.vm.context, &createDirectoryBind, "createDirectory", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "createDirectory", fn, 0);
    // isPath(path: string): bool
    fn = JS_NewCFunction(engine.vm.context, &isPathBind, "isPath", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "isPath", fn, 0);
    // isFile(path: string): bool
    fn = JS_NewCFunction(engine.vm.context, &isFileBind, "isFile", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "isFile", fn, 0);
    // isDirectory(path: string): bool
    fn = JS_NewCFunction(engine.vm.context, &isDirectoryBind, "isDirectory", 0);
    JS_DefinePropertyValueStr(engine.vm.context, proto, "isDirectory", fn, 0);
    engine.vm.addExport("System", proto);
}