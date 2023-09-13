#include "renderer_binding.hpp"
#include "quickjs.h"
#include "engine/engine.hpp"
#include <iostream>

JSValue setClearColorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    auto r = help.getFloat64();
    auto g = help.getFloat64();
    auto b = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.setClearColor(r, g, b);
    return JS_UNDEFINED;
}

JSValue setLoadImage(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    auto path = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto handle = engine.renderer.loadImage(path.c_str());
    if(handle == -1){
        JS_ThrowReferenceError(ctx, "unable to load image at path!");
        return JS_EXCEPTION;
    }
    auto res = JS_NewInt32(ctx, handle);
    return res;
}

JSValue setDrawImage(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    float x,y,width,height,sx,sy,sw,sh;
    int handle = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    if(!engine.renderer.textureStore.has(handle)){
        JS_ThrowReferenceError(ctx, "invalid texture handle!");
        return JS_EXCEPTION;
    }
    auto tex = engine.renderer.textureStore.get(handle);
    x = help.getFloat64();
    y = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    width = help.hasArgs() ? help.getFloat64() : tex->width;
    height = help.hasArgs() ? help.getFloat64() : tex->height;
    sx = help.hasArgs() ? help.getFloat64() : 0; 
    sy = help.hasArgs() ? help.getFloat64() : 0;
    sw = help.hasArgs() ? help.getFloat64() : width;
    sh = help.hasArgs() ? help.getFloat64() : height;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.drawImage(handle, x,y,width,height,sx,sy,sw,sh);
    return JS_UNDEFINED;
}

int setRenderMod(JSContext* ctx, JSModuleDef* mod){
    std::string name = "setClearColor";
    auto fn = JS_NewCFunction(engine.vm.context, &setClearColorBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "loadImage";
    fn = JS_NewCFunction(engine.vm.context, &setLoadImage, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "drawImage";
    fn = JS_NewCFunction(engine.vm.context, &setDrawImage, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    return 0;
}

void bindRenderer(bool* hasError){
    if(*hasError) return;
    auto renderMod = JS_NewCModule(engine.vm.context, "cleo-renderer", &setRenderMod);
    JS_AddModuleExport(engine.vm.context, renderMod, "setClearColor");
    JS_AddModuleExport(engine.vm.context, renderMod, "loadImage");
    JS_AddModuleExport(engine.vm.context, renderMod, "drawImage");
}