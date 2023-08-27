#include "vm.hpp"
#include <iostream>
#include "../engine/engine.hpp"

JSValue setInitBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    engine.vm.initFn = help.getFunction();
    if(help.hasError) return JS_EXCEPTION;
    return JS_UNDEFINED;
}

int setMainMod(JSContext* ctx, JSModuleDef* mod){
    auto setInitFn = JS_NewCFunction(ctx, &setInitBind, "setInit", 0);
    JS_SetModuleExport(ctx, mod, "setInit", setInitFn);
    return 0;
}

VM::~VM(){
    if(!initialized) return;
    JS_FreeContext(context);
    JS_FreeRuntime(runtime);
}

void VM::init(bool* hasError){
    if(*hasError) return;
    runtime = JS_NewRuntime();
    context = JS_NewContext(runtime);
    initialized = true;
}

void VM::bind(std::string src){
    auto mainMod = JS_NewCModule(context, "cleo", &setMainMod);
    JS_AddModuleExport(context, mainMod, "setInit");
    JS_AddModuleExport(context, mainMod, "setLoop");
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
JSValue FnHelp::getFunction(){
    auto val = next();
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