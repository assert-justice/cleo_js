#include "fn_help.hpp"

FnHelp::FnHelp(JSContext* ctx, int argc, JSValue* argv){
    this->ctx = ctx; this->argc = argc; this->argv = argv;
}

JSValue FnHelp::next(){
    if(hasError) return JS_UNDEFINED;
    if(argIdx >= argc) {
        // std::cout << argIdx << std::endl;
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