#include "bind_graphics.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include "object_help.hpp"
#include <iostream>

typedef struct {
    int id;
    Texture* texture;
} JSTextureClass;

static JSClassID jsTextureClassId;

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

static JSTextureClass* getTexture(JSValue val){
    auto s = (JSTextureClass*)JS_GetOpaque(val, jsTextureClassId);
    if(!s){
        JS_ThrowReferenceError(engine.vm.context, "method referenced before initialization!");
        return nullptr;
    }
    return s;
}

static JSValue rendererInitalized(JSContext* ctx){
    if(!engine.window.initialized){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

static JSValue setClearColorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto r = help.getFloat64();
    auto g = help.getFloat64();
    auto b = help.getFloat64();
    auto a = help.hasArgs() ? help.getFloat64() : 1.0;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.setClearColor(r, g, b, a);
    return JS_UNDEFINED;
}

static JSValue clearBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    engine.renderer.clear();
    return JS_UNDEFINED;
}

static JSValue getTextureWidthBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewFloat64(engine.vm.context, s->texture->width);
}
static JSValue getTextureHeightBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewFloat64(engine.vm.context, s->texture->height);
}

static JSValue textureFromFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
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

static JSValue textureConstructorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto width = help.getFloat64();
    auto height = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    auto textureId = engine.renderer.newTexture(width, height, NULL);
    // auto textureId = engine.renderer.loadImage(path.c_str());
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "unable to load image at path!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.textureStore.get(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}

static JSValue drawImageBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    float x,y,scaleX,scaleY,sx,sy,sw,sh;
    auto tex = s->texture;
    x = help.getFloat64();
    y = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    ObjectHelp objHelp(ctx, thisVal);
    scaleX = objHelp.getNumber("scaleX", 1.0);
    scaleY = objHelp.getNumber("scaleY", 1.0);
    sx = objHelp.getNumber("sx", 0.0);
    sy = objHelp.getNumber("sy", 0.0);
    sw = objHelp.getNumber("sw", tex->width);
    sh = objHelp.getNumber("sh", tex->height);
    glm::mat4 spriteTransform = glm::mat4(1.0f);
    spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
    spriteTransform = glm::scale(spriteTransform, glm::vec3(tex->width * scaleX,tex->height * scaleY, 0.0f));

    // width = help.hasArgs() ? help.getFloat64() : tex->width;
    // height = help.hasArgs() ? help.getFloat64() : tex->height;
    // sx = help.hasArgs() ? help.getFloat64() : 0; 
    // sy = help.hasArgs() ? help.getFloat64() : 0;
    // sw = help.hasArgs() ? help.getFloat64() : tex->width;
    // sh = help.hasArgs() ? help.getFloat64() : tex->height;
    // std::cout << "sw: " << sw << std::endl;
    if(objHelp.hasError) return JS_EXCEPTION;
    // engine.renderer.drawImage(s->id, x,y,tex->width * scaleX,tex->height * scaleY,sx,sy,sw,sh);
    engine.renderer.drawImage(s->id,spriteTransform ,sx,sy,sw,sh);
    return JS_UNDEFINED;
}

static JSValue setRenderTargetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    engine.renderer.setTarget(s->texture);
    return JS_UNDEFINED;
}
static JSValue resetRenderTargetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(rendererInitalized(ctx))) return JS_EXCEPTION;
    engine.renderer.setTarget(nullptr);
    return JS_UNDEFINED;
}

void bindGraphics(){
    JSValue proto, textureProto;
    JSValue fn;
    auto ctx = engine.vm.context;

    proto = JS_NewObject(ctx);

    /* the class ID is created once */
    JS_NewClassID(&jsTextureClassId);
    /* the class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), jsTextureClassId, &jsTextureClassDef);
    textureProto = JS_NewObject(ctx);
    JS_SetClassProto(ctx, jsTextureClassId, textureProto);
    // static fromFile(path: string)
    fn = JS_NewCFunction(ctx, &textureFromFileBind, "fromFile", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "fromFile", fn, 0);
    // get width: number
    fn = JS_NewCFunction(ctx, &getTextureWidthBind, "getTextureWidth", 0);
    JS_DefineProperty(ctx, textureProto, JS_NewAtom(ctx, "width"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // get height: number
    fn = JS_NewCFunction(ctx, &getTextureHeightBind, "getTextureHeight", 0);
    JS_DefineProperty(ctx, textureProto, JS_NewAtom(ctx, "height"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // draw()
    fn = JS_NewCFunction(ctx, &drawImageBind, "drawImage", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "draw", fn, 0);
    // setTarget()
    fn = JS_NewCFunction(ctx, &setRenderTargetBind, "setRenderTarget", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "setTarget", fn, 0);
    fn = JS_NewCFunction(ctx, &resetRenderTargetBind, "resetRenderTarget", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "resetTarget", fn, 0);
    // constructor(width, height)
    // not working, getting 'not a constructor' error for some reason
    // fn = JS_NewCFunction(ctx, &textureConstructorBind, "textureConstructor", 0);
    // JS_SetConstructorBit(ctx, fn, true);
    // JS_SetConstructor(ctx, fn, textureProto);
    // JS_FreeValue(ctx, fn);
    // new(with: number, height: number)
    fn = JS_NewCFunction(ctx, &textureConstructorBind, "textureConstructor", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "new", fn, 0);
    // setting the class prototype consumes the prototype object so we need to reacquire it
    textureProto = JS_GetClassProto(ctx, jsTextureClassId);
    JS_DefinePropertyValueStr(ctx, proto, "Texture", textureProto, 0);
    // setClearColor
    fn = JS_NewCFunction(ctx, &setClearColorBind, "setClearColor", 0);
    JS_DefinePropertyValueStr(ctx, proto, "setClearColor", fn, 0);
    // clear
    fn = JS_NewCFunction(ctx, &clearBind, "clear", 0);
    JS_DefinePropertyValueStr(ctx, proto, "clear", fn, 0);
    engine.vm.addExport("Graphics", proto);
}