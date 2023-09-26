#include "vm.hpp"
#include <iostream>
#include "../engine/engine.hpp"
#include "utils/fs.hpp"
#include "fn_help.hpp"

JSModuleDef *jsModuleLoader(JSContext *ctx,
                              const char *module_name, void *opaque)
{
    JSModuleDef *m;
    bool hasError = false;
    auto src = readFile(&hasError, module_name);
    if(hasError){
        JS_ThrowReferenceError(ctx, "could not load module filename '%s'",
                                   module_name);
            return NULL;
    }
    JSValue func_val = JS_Eval(ctx, src.c_str(), src.length(), module_name, 
        JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY);
    /* compile the module */
    if (JS_IsException(func_val))
        return NULL;
    /* XXX: could propagate the exception */
    // js_module_set_import_meta(ctx, func_val, TRUE, FALSE);
    /* the module is already referenced, so we must free it */
    m = (JSModuleDef*)JS_VALUE_GET_PTR(func_val);
    JS_FreeValue(ctx, func_val);
    return m;
}

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
    JS_SetModuleLoaderFunc(runtime, NULL, jsModuleLoader, NULL);
    // JS_DefineProperty()
    initialized = true;
}

void VM::bind(bool* hasError, std::string src){
    auto mainMod = JS_NewCModule(context, "cleo", &setMainMod);
    // JS_seto
    JS_AddModuleExport(context, mainMod, "setInit");
    JS_AddModuleExport(context, mainMod, "setUpdate");
    JS_AddModuleExport(context, mainMod, "println");
    auto val = JS_Eval(context, src.c_str(), src.size(), "main", JS_EVAL_TYPE_MODULE);
    if(!isException(context, val)) JS_FreeValue(context, val);
    else{*hasError = true;}
}

void VM::launch(bool* hasError){
    if(JS_IsUndefined(initFn)){
        *hasError = true;
        return;
    }
    auto val = JS_Call(context, initFn, JS_UNDEFINED, 0, NULL);
    if(isException(context, val)) *hasError = true;
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

