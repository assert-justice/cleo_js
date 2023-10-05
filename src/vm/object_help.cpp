#include "object_help.hpp"

ObjectHelp::ObjectHelp(JSContext* ctx, JSValue obj){
    this->ctx = ctx; 
    this->obj = obj; // maybe should dup, dunno
}

JSValue ObjectHelp::getVal(const char* name){
    if(!JS_HasProperty(ctx, obj, JS_NewAtom(ctx, name))) return JS_UNDEFINED;
    return JS_GetPropertyStr(ctx, obj, name);
}
JSValue ObjectHelp::getValRequired(const char* name){
    if(!JS_HasProperty(ctx, obj, JS_NewAtom(ctx, name))) {
        JS_ThrowTypeError(ctx, "required field '%s' does not exist!", name);
        return JS_EXCEPTION;
    }
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
void ObjectHelp::setNumber(const char* name, double val){
    auto num = JS_NewFloat64(ctx, val);
    JS_DefinePropertyValueStr(ctx, obj, name, num, 0);
}
bool ObjectHelp::getBool(const char* name, bool defaultVal){
    auto val = getVal(name);
    if(JS_IsUndefined(val)) return defaultVal;
    if(!JS_IsBool(val)){
        JS_ThrowTypeError(ctx, "field '%s' is not a boolean!", name);
        hasError = true;
        return defaultVal;
    }
    bool res;
    JS_ToBool(ctx, val);
    return res;
}
void ObjectHelp::setBool(const char* name, bool val){
    auto num = JS_NewBool(ctx, val);
    JS_DefinePropertyValueStr(ctx, obj, name, num, 0);
}
