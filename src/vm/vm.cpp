#include "vm.hpp"
#include <iostream>
#include "engine/engine.hpp"
#include "utils/fs.hpp"
#include "fn_help.hpp"
#include "vm/bind_game.hpp"
#include "vm/bind_system.hpp"
#include "vm/bind_input.hpp"
#include "vm/bind_window.hpp"
#include "vm/bind_graphics.hpp"

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

// JSValue printlnBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
//     FnHelp help(ctx, argc, argv);
//     auto str = help.getString();
//     if(help.hasError) return JS_EXCEPTION;
//     std::cout << str << std::endl;
//     return JS_UNDEFINED;
// }

int setMainMod(JSContext* ctx, JSModuleDef* mod){
    for(auto it=engine.vm.exports.begin();it!=engine.vm.exports.end();it++) {
        // std::cout << "grrr\n";
        JS_SetModuleExport(ctx, mod, 
            it.base()->first.c_str(), 
            it.base()->second);
    }
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
    bindInput();
    bindGame();
    bindSystem();
    bindWindow();
    bindGraphics();
    // JS_DefineProperty()
    // auto printlnFn = JS_NewCFunction(context, &printlnBind, "println", 0);
    // addExport("println", printlnFn);
    // JS_FreeValue(context, printlnFn);
    initialized = true;
}

void VM::bind(bool* hasError, std::string src){
    auto mainMod = JS_NewCModule(context, "cleo", &setMainMod);
    // std::cout << "len: " << exports.size() << std::endl;
    for(auto it=exports.begin();it!=exports.end();it++) {
        JS_AddModuleExport(context, mainMod, it.base()->first.c_str());
    }
    auto val = JS_Eval(context, src.c_str(), src.size(), "main", JS_EVAL_TYPE_MODULE);
    if(!isException(context, val)) JS_FreeValue(context, val);
    else{*hasError = true;}
}

void VM::launch(bool* hasError){
    // not having an init function is fine actually
    // if(JS_IsUndefined(initFn)){
    //     *hasError = true;
    //     return;
    // }
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

void VM::addExport(std::string name, JSValue value){
    exports.push_back(std::pair<std::string, JSValue>(name, value));
}