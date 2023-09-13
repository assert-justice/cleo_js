#include "vm.hpp"
#include <iostream>
#include "../engine/engine.hpp"

JSValue setInitBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    engine.vm.initFn = JS_DupValue(ctx, fn);
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

JSValue setUpdateBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto fn = help.getFunction();
    engine.vm.updateFn = JS_DupValue(ctx, fn);
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

JSValue printlnBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto str = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    std::cout << str << std::endl;
    return JS_UNDEFINED;
}

int setMainMod(JSContext* ctx, JSModuleDef* mod){
    auto setInitFn = JS_NewCFunction(ctx, &setInitBind, "setInit", 0);
    JS_SetModuleExport(ctx, mod, "setInit", setInitFn);
    auto setUpdateFn = JS_NewCFunction(ctx, &setUpdateBind, "setUpdate", 0);
    JS_SetModuleExport(ctx, mod, "setUpdate", setUpdateFn);
    auto printlnFn = JS_NewCFunction(ctx, &printlnBind, "println", 0);
    JS_SetModuleExport(ctx, mod, "println", printlnFn);
    return 0;
}

VM::~VM(){
    if(!initialized) return;
    JS_FreeValue(context, initFn);
    JS_FreeValue(context, updateFn);
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);
}

void VM::init(bool* hasError){
    if(*hasError) return;
    runtime = JS_NewRuntime();
    context = JS_NewContext(runtime);
    initialized = true;
}

void VM::bind(bool* hasError, std::string src){
    auto mainMod = JS_NewCModule(context, "cleo", &setMainMod);
    // JS_seto
    JS_AddModuleExport(context, mainMod, "setInit");
    JS_AddModuleExport(context, mainMod, "setUpdate");
    JS_AddModuleExport(context, mainMod, "println");
    auto val = JS_Eval(context, src.c_str(), src.size(), "temp", JS_EVAL_TYPE_MODULE);
    if(!isException(context, val)) JS_FreeValue(context, val);
    else{*hasError = true;}
}

void VM::launch(){
    if(!JS_IsUndefined(initFn)) JS_Call(context, initFn, JS_UNDEFINED, 0, NULL);
}

void VM::update(double dt){
    if(!JS_IsUndefined(updateFn)){
        auto val = JS_NewFloat64(context, dt);
        JS_Call(context, updateFn, JS_UNDEFINED, 1, &val);
        // dunno if this is needed
        JS_FreeValue(context, val);
    }
}

bool isException(JSContext* context, JSValue val){
    if(JS_IsException(val)){
        auto exception = JS_GetException(context);
        auto str = JS_ToCString(context, exception);
        std::cerr << str << std::endl;
        if(JS_IsError(context, exception)){
            auto stack = JS_GetPropertyStr(context, exception, "stack");
            if(!JS_IsUndefined(stack)){
                auto stackStr = JS_ToCString(context, stack);
                std::cerr << stackStr << std::endl;
                JS_FreeCString(context, stackStr);
            }
            JS_FreeValue(context, stack);
        }
        JS_FreeCString(context, str);
        JS_FreeValue(context, exception);
        return true;
    }
    return false;
}

FnHelp::FnHelp(JSContext* ctx, int argc, JSValue* argv){
    this->ctx = ctx; this->argc = argc; this->argv = argv;
}

JSValue FnHelp::next(){
    if(hasError) return JS_UNDEFINED;
    if(argIdx >= argc) {
        std::cout << argIdx << std::endl;
        hasError = true;
        JS_ThrowTypeError(ctx, "airity error, not enough arguments!");
        return JS_EXCEPTION;
    }
    argIdx++;
    return argv[argIdx - 1];
}

double FnHelp::getFloat64(){
    auto val = next();
    if(hasError) return 0;
    if(!JS_IsNumber(val)) {hasError = true; return 0;}
    double res = 0;
    JS_ToFloat64(ctx, &res, val);
    return res;
}

int FnHelp::getInt(){
    auto val = next();
    if(hasError) return 0;
    if(!JS_IsNumber(val)) {hasError = true; return 0;}
    int res = 0;
    JS_ToInt32(ctx, &res, val);
    return res;
}

JSValue FnHelp::getFunction(){
    auto val = next();
    // JS_ValueToAtom
    if(hasError) return JS_EXCEPTION;
    if(!JS_IsFunction(ctx, val)) {hasError = true; return JS_EXCEPTION;}
    return val;
}

std::string FnHelp::getString(){
    auto val = next();
    if(hasError) return "";
    if(!JS_IsString(val)){hasError = true; return "";}
    auto str = JS_ToCString(ctx, val);
    std::string res(str);
    JS_FreeCString(ctx, str);
    return res;
}

bool FnHelp::hasArgs(){
    return argIdx < argc;
}