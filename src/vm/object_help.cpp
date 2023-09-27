#include "object_help.hpp"

ObjectHelp::ObjectHelp(JSContext* ctx, JSValue obj){
    this->ctx = ctx; 
    this->obj = obj; // maybe should dup, dunno
}

JSValue ObjectHelp::getVal(const char* name){
    if(!JS_HasProperty(ctx, obj, JS_NewAtom(ctx, name))) return JS_UNDEFINED;
    return JS_GetPropertyStr(ctx, obj, name);
}

double ObjectHelp::getNumber(const char* name, double defaultVal){
    auto val = getVal(name);
    if(JS_IsUndefined(val)) return defaultVal;
    if(!JS_IsNumber(val)){
        JS_ThrowTypeError(ctx, "field '%s' is not a number!", name);
        hasError = true;
        return defaultVal;
    }
    double res;
    JS_ToFloat64(ctx, &res, val);
    return res;
}