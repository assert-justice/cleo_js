#include "renderer_binding.hpp"
#include "vm/fn_help.hpp"
#include "engine/engine.hpp"
#include <iostream>

typedef struct {
    int id;
    Texture* texture;
} JSTextureClass;

JSClassID jsTextureClassId;

static void jsTextureClassFinalizer(JSRuntime *rt, JSValue val)
{
    JSTextureClass* s = (JSTextureClass*)JS_GetOpaque(val, jsTextureClassId);
    if (s) {
        if(engine.renderer.isInitialized())
            engine.renderer.freeTexture(s->id);
        // std::cout << "freed texture\n";
        js_free_rt(rt, s);
    }
}

static JSClassDef jsTextureClassDef = {
    "Texture",
    .finalizer = jsTextureClassFinalizer,
}; 

static JSValue newJSTextureHandle(JSContext *ctx, int id, Texture* texture)
{
    JSTextureClass* s;
    JSValue obj;
    obj = JS_NewObjectClass(ctx, jsTextureClassId);
    if (JS_IsException(obj))
        return obj;
    s = (JSTextureClass*)js_mallocz(ctx, sizeof(*s));
    if (!s) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }
    s->id = id;
    s->texture = texture;
    JS_SetOpaque(obj, s);
    return obj;
}

JSValue setCameraPositionBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    auto x = help.getFloat64();
    auto y = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.setCameraPosition(x, y);
    return JS_UNDEFINED;
}

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

JSValue loadImageBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    auto path = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto textureId = engine.renderer.loadImage(path.c_str());
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "unable to load image at path!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.textureStore.get(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}

JSValue drawImageBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    float x,y,width,height,sx,sy,sw,sh;
    auto handle = help.next();
    if(help.hasError) return JS_EXCEPTION;
    auto s = (JSTextureClass*)JS_GetOpaque2(ctx, handle, jsTextureClassId);
    if(!s){
        JS_ThrowReferenceError(ctx, "invalid texture handle!");
        return JS_EXCEPTION;
    }
    auto tex = s->texture;
    x = help.getFloat64();
    y = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    width = help.hasArgs() ? help.getFloat64() : tex->width;
    height = help.hasArgs() ? help.getFloat64() : tex->height;
    sx = help.hasArgs() ? help.getFloat64() : 0; 
    sy = help.hasArgs() ? help.getFloat64() : 0;
    sw = help.hasArgs() ? help.getFloat64() : tex->width;
    sh = help.hasArgs() ? help.getFloat64() : tex->height;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.drawImage(s->id, x,y,width,height,sx,sy,sw,sh);
    return JS_UNDEFINED;
}

JSValue setRenderTargetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    if(argc == 0){
        // reset render target
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return JS_UNDEFINED;
    }
    FnHelp help(ctx, argc, argv);
    auto handle = help.next();
    if(help.hasError) return JS_EXCEPTION;
    auto s = (JSTextureClass*)JS_GetOpaque2(ctx, handle, jsTextureClassId);
    if(!s){
        JS_ThrowReferenceError(ctx, "invalid texture handle!");
        return JS_EXCEPTION;
    }
    s->texture->useTarget();
    return JS_UNDEFINED;
}

int setRenderMod(JSContext* ctx, JSModuleDef* mod){
    JSValue proto;
    /* the class ID is created once */
    JS_NewClassID(&jsTextureClassId);
    /* the class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), jsTextureClassId, &jsTextureClassDef);
    proto = JS_NewObject(ctx);
    JS_SetClassProto(ctx, jsTextureClassId, proto);
    std::string name = "setClearColor";
    auto fn = JS_NewCFunction(engine.vm.context, &setClearColorBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "loadImage";
    fn = JS_NewCFunction(engine.vm.context, &loadImageBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "drawImage";
    fn = JS_NewCFunction(engine.vm.context, &drawImageBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "setRenderTarget";
    fn = JS_NewCFunction(engine.vm.context, &setRenderTargetBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    name = "setCameraPosition";
    fn = JS_NewCFunction(engine.vm.context, &setCameraPositionBind, name.c_str(), 0);
    JS_SetModuleExport(engine.vm.context, mod, name.c_str(), fn);
    return 0;
}

void bindRenderer(bool* hasError){
    if(*hasError) return;
    auto renderMod = JS_NewCModule(engine.vm.context, "cleo-renderer", &setRenderMod);
    JS_AddModuleExport(engine.vm.context, renderMod, "setClearColor");
    JS_AddModuleExport(engine.vm.context, renderMod, "loadImage");
    JS_AddModuleExport(engine.vm.context, renderMod, "drawImage");
    JS_AddModuleExport(engine.vm.context, renderMod, "setRenderTarget");
    JS_AddModuleExport(engine.vm.context, renderMod, "setCameraPosition");
}