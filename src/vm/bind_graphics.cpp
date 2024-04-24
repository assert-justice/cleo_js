#include "bind_graphics.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include "object_help.hpp"
#include <iostream>

typedef struct {
    int id;
    Texture* texture;
} JSTextureClass;

typedef struct {
    int id;
    Shader* shader;
} JSShaderClass;

typedef struct {
    int id;
    Mesh* mesh;
} JSMeshClass;

static JSClassID jsTextureClassId, jsShaderClassId, jsMeshClassId;

static void jsTextureClassFinalizer(JSRuntime *rt, JSValue val)
{
    JSTextureClass* s = (JSTextureClass*)JS_GetOpaque(val, jsTextureClassId);
    if (s) {
        if(engine.renderer.isInitialized())
            engine.renderer.freeTexture(s->id);
        js_free_rt(rt, s);
    }
}
static void jsShaderClassFinalizer(JSRuntime *rt, JSValue val)
{
    JSShaderClass* s = (JSShaderClass*)JS_GetOpaque(val, jsShaderClassId);
    if (s) {
        if(engine.renderer.isInitialized())
            engine.renderer.freeShader(s->id);
        js_free_rt(rt, s);
    }
}
static void jsMeshClassFinalizer(JSRuntime *rt, JSValue val)
{
    JSMeshClass* s = (JSMeshClass*)JS_GetOpaque(val, jsMeshClassId);
    if (s) {
        if(engine.renderer.isInitialized())
            engine.renderer.freeMesh(s->id);
        js_free_rt(rt, s);
    }
}

static JSClassDef jsTextureClassDef = {
    "Texture",
    .finalizer = jsTextureClassFinalizer,
}; 
static JSClassDef jsShaderClassDef = {
    "Shader",
    .finalizer = jsShaderClassFinalizer,
}; 
static JSClassDef jsMeshClassDef = {
    "Mesh",
    .finalizer = jsMeshClassFinalizer,
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
static JSValue newJSShaderHandle(JSContext *ctx, int id, Shader* shader)
{
    JSShaderClass* s;
    JSValue obj;
    obj = JS_NewObjectClass(ctx, jsShaderClassId);
    if (JS_IsException(obj))
        return obj;
    s = (JSShaderClass*)js_mallocz(ctx, sizeof(*s));
    if (!s) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }
    s->id = id;
    s->shader = shader;
    JS_SetOpaque(obj, s);
    return obj;
}
static JSValue newJSMeshHandle(JSContext *ctx, int id, Mesh* mesh)
{
    JSMeshClass* s;
    JSValue obj;
    obj = JS_NewObjectClass(ctx, jsMeshClassId);
    if (JS_IsException(obj))
        return obj;
    s = (JSMeshClass*)js_mallocz(ctx, sizeof(*s));
    if (!s) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }
    s->id = id;
    s->mesh = mesh;
    JS_SetOpaque(obj, s);
    return obj;
}

static JSTextureClass* getTexture(JSValue val){
    auto s = (JSTextureClass*)JS_GetOpaque(val, jsTextureClassId);
    if(!s){
        JS_ThrowReferenceError(engine.vm.context, "invalid texture handle!");
        return nullptr;
    }
    return s;
}
static JSShaderClass* getShader(JSValue val){
    auto s = (JSShaderClass*)JS_GetOpaque(val, jsShaderClassId);
    if(!s){
        JS_ThrowReferenceError(engine.vm.context, "invalid shader handle!");
        return nullptr;
    }
    return s;
}
static JSMeshClass* getMesh(JSValue val){
    auto s = (JSMeshClass*)JS_GetOpaque(val, jsMeshClassId);
    if(!s){
        JS_ThrowReferenceError(engine.vm.context, "invalid mesh handle!");
        return nullptr;
    }
    return s;
}

static bool rendererInitalized(JSContext* ctx){
    if(!engine.window.isInitalized() || !engine.renderer.isInitialized()){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return false;
    }
    return true;
}

static JSValue setClearColorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto r = help.getFloat64() / 255;
    auto g = help.getFloat64() / 255;
    auto b = help.getFloat64() / 255;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.setClearColor(r, g, b);
    return JS_UNDEFINED;
}

static JSValue clearBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    engine.renderer.clear();
    return JS_UNDEFINED;
}

static JSValue getTextureWidthBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewFloat64(engine.vm.context, s->texture->width);
}
static JSValue getTextureHeightBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewFloat64(engine.vm.context, s->texture->height);
}

static JSValue textureConstructorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto width = help.getInt();
    auto height = help.getInt();
    auto array = JS_UNDEFINED;
    if(help.hasArgs()) array = help.next();
    if(help.hasError) return JS_EXCEPTION;
    int textureId = -1;
    if(JS_IsUndefined(array)){
        textureId = engine.renderer.newTexture(width, height, NULL);
    }
    else{
        size_t length;
        auto data = (unsigned char*)JS_GetArrayBuffer(ctx, &length, array);
        // Note: the data returned by JS_GetArrayBuffer is managed by QuickJS and should not be freed.
        if(data == NULL){
            JS_ThrowTypeError(ctx, "Cannot read texture data!");
            return JS_EXCEPTION;
        }
        else if(length != width * height * 4){
            JS_ThrowTypeError(ctx, "Invalid buffer length");
            return JS_EXCEPTION;
        }
        textureId = engine.renderer.newTexture(width, height, data);
    }
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "Unable to create an image!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.getTexture(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}

static JSValue textureFromArrayBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto width = help.getInt();
    auto height = help.getInt();
    auto array = help.getArray();
    if(help.hasError) return JS_EXCEPTION;
    int textureId = -1;
    auto lengthValue = JS_GetProperty(ctx, array, JS_NewAtom(ctx, "length"));
    auto length = 0;
    JS_ToInt32(ctx, &length, lengthValue);
    printf("length: %i\n", length);
    if(length != width * height * 4){
        JS_ThrowTypeError(ctx, "type error, invalid array length for texture");
        return JS_EXCEPTION;
    }
    auto data = (unsigned char*)malloc(length);
    for (int idx = 0; idx < length; idx++)
    {
        auto str = std::to_string(idx);
        auto jsVal = JS_GetProperty(ctx, array, JS_NewAtom(ctx, str.c_str()));
        if(!JS_IsNumber(jsVal)){
            JS_ThrowTypeError(ctx, "type error, found non-numeric number in image data array!");
            free(data);
            return JS_EXCEPTION;
        }
        int val = 0;
        JS_ToInt32(ctx, &val, jsVal);
        data[idx] = (unsigned char) val;
    }
    textureId = engine.renderer.newTexture(width, height, data);
    free(data);
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "unable to create an image!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.getTexture(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}
static JSValue textureFromColorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto width = help.getInt();
    auto height = help.getInt();
    auto red = help.getInt();
    auto green = help.getInt();
    auto blue = help.getInt();
    auto alpha = help.getInt();
    if(help.hasError) return JS_EXCEPTION;
    int textureId = -1;
    int length = width * height * 4;
    auto data = (unsigned char*)malloc(length);
    for(int idx = 0; idx < length; idx+=4){
        data[idx] = red;
        data[idx+1] = green;
        data[idx+2] = blue;
        data[idx+3] = alpha;
    }
    textureId = engine.renderer.newTexture(width, height, data);
    free(data);
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "unable to create an image!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.getTexture(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}

static JSValue textureFromFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto path = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto textureId = engine.renderer.loadImage(path.c_str());
    if(textureId == -1){
        JS_ThrowReferenceError(ctx, "unable to load image at path!");
        return JS_EXCEPTION;
    }
    auto texturePtr = engine.renderer.getTexture(textureId);
    auto res = newJSTextureHandle(ctx, textureId, texturePtr);
    return res;
}

static JSValue shaderConstructorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto vertexSource = help.getString();
    auto fragmentSource = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto shaderId = engine.renderer.newShader(vertexSource.c_str(), fragmentSource.c_str());
    if(shaderId == -1){
        JS_ThrowReferenceError(ctx, "unable to compile shader!");
        return JS_EXCEPTION;
    }
    auto shaderPtr = engine.renderer.getShader(shaderId);
    auto res = newJSShaderHandle(ctx, shaderId, shaderPtr);
    return res;
}

static JSValue shaderGetUniformLocationBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto name = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto s = getShader(thisVal);
    if(!s) return JS_EXCEPTION; // Error message handled in getShader
    auto shader = s->shader;
    auto location = shader->getUniformLocation(name.c_str());
    return JS_NewInt32(ctx, location);
}
static JSValue shaderGetAttribLocationBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto name = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto s = getShader(thisVal);
    if(!s) return JS_EXCEPTION; // Error message handled in getShader
    auto shader = s->shader;
    auto location = shader->getAttribLocation(name.c_str());
    return JS_NewInt32(ctx, location);
}

static JSValue meshConstructorBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto s = getShader(help.next());
    if(!s) return JS_EXCEPTION;
    Shader* shader = s->shader;
    int vertexCount = help.getInt();
    int vertexSize = help.getInt();
    auto attribArray = help.getArray();
    auto vertexArray = help.getArray();
    if(help.hasError) return JS_EXCEPTION;
    std::vector<std::pair<int, const char*>> attributes;
    ObjectHelp arrHelp(ctx, attribArray);
    int length = arrHelp.getNumber("length");
    // TODO: Set some maximum mesh size
    // auto errMessage = "All elements of attribute array must be of the form [number, string]";
    int offset = 0;
    for(int idx = 0; idx < length; idx++){
        auto obj = arrHelp.getVal(std::to_string(idx).c_str());
        if(!JS_IsArray(ctx, obj)){
            JS_ThrowTypeError(ctx, "All elements of attribute array must be of the form [number, string]");
            return JS_EXCEPTION;
        }
        ObjectHelp entryHelp(ctx, obj);
        bool hasError = false;
        if(entryHelp.getNumber("length") != 2) hasError = true;
        else if(!JS_IsNumber(entryHelp.getValRequired("0"))) hasError = true;
        else if(!JS_IsString(entryHelp.getValRequired("1"))) hasError = true;
        if(hasError){
            JS_ThrowTypeError(ctx, "All elements of attribute array must be of the form [number, string]");
            return JS_EXCEPTION;
        }
        if(entryHelp.hasError) return JS_EXCEPTION;
        int size = entryHelp.getNumber("0");
        offset += size;
        if(offset > vertexSize){
            JS_ThrowRangeError(ctx, "The combined size of the attributes is greater than the vertex size!");
            return JS_EXCEPTION;
        }
        attributes.push_back(std::pair<int, const char*>(size, entryHelp.getString("1").c_str()));
        JS_FreeValue(ctx, obj);
    }
    // return JS_UNDEFINED;
    ObjectHelp dataHelp(ctx, vertexArray);
    int dataLength = dataHelp.getNumber("length");
    std::vector<float> data;
    for(int idx = 0; idx < dataLength; idx++){
        float val = dataHelp.getNumber(std::to_string(idx).c_str());
        if(dataHelp.hasError){}
        data.push_back(val); 
    }
    int meshId = engine.renderer.newMesh(shader, data.data(), vertexCount, vertexSize, attributes);
    Mesh* mesh = engine.renderer.getMesh(meshId);
    return newJSMeshHandle(ctx, meshId, mesh);
}

static JSValue drawImageBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    if(!engine.renderer.isEnabled()){
        JS_ThrowReferenceError(ctx, "method called outside of draw function!");
        return JS_EXCEPTION;
    }
    FnHelp help(ctx, argc, argv);
    float x,y,width,height,sx,sy,sw,sh,ox, oy, angle;
    x = help.getFloat64();
    y = help.getFloat64();
    auto handle = help.hasArgs() ? help.next() : JS_UNDEFINED;
    auto s = getTexture(thisVal);
    if(!s) return JS_EXCEPTION; // Error message handled in getTexture
    auto tex = s->texture;
    if(help.hasError) return JS_EXCEPTION;
    ObjectHelp objHelp(ctx, handle);
    width = objHelp.getNumber("width", tex->width);
    height = objHelp.getNumber("height", tex->height);
    sx = objHelp.getNumber("sx", 0.0);
    sy = objHelp.getNumber("sy", 0.0);
    sw = objHelp.getNumber("sw", tex->width);
    sh = objHelp.getNumber("sh", tex->height);
    ox = objHelp.getNumber("ox", 0.0);
    oy = objHelp.getNumber("oy", 0.0);
    angle = objHelp.getNumber("angle", 0.0);
    bool visible = objHelp.getBool("visible", true);
    if(objHelp.hasError) return JS_EXCEPTION;
    if(!visible) return JS_UNDEFINED;
    // calculate sprite transform
    // scale, offset, rotate, scale2, translate
    // have to do it in reverse because matrix math is Like That
    glm::mat4 spriteTransform = glm::mat4(1.0f);
    spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
    spriteTransform = glm::rotate(spriteTransform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    spriteTransform = glm::translate(spriteTransform, glm::vec3(-ox/sw*width, -oy/sh*height, 0.0f));
    spriteTransform = glm::scale(spriteTransform, glm::vec3(width,height, 0.0f));

    // calculate texture coord transform
    // add a tiny offset to texture sampling to prevent bleed
    float sigma = 0.001;
    // scale, translate
    glm::mat4 coordTransform = glm::mat4(1.0f);
    coordTransform = glm::translate(coordTransform, glm::vec3(glm::vec3(sx/tex->width+sigma, sy/tex->height+sigma, 0.0)));
    coordTransform = glm::scale(coordTransform, glm::vec3(sw/tex->width - 2 * sigma, sh/tex->height - 2 * sigma, 0.0));

    engine.renderer.drawImage(s->id,spriteTransform,coordTransform);
    return JS_UNDEFINED;
}

static JSValue pushRenderTargetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto texture = help.next();
    if(help.hasError) return JS_EXCEPTION;
    auto s = getTexture(texture);
    if(!s) return JS_EXCEPTION;
    bool pushSucceeded = engine.renderer.pushRenderTarget(s->id);
    if(!pushSucceeded) {
        JS_ThrowReferenceError(ctx, "Invalid texture handle.");
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

static JSValue popRenderTargetBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    auto id = engine.renderer.popRenderTarget();
    if(id == -1) {
        JS_ThrowReferenceError(ctx, "Cannot pop render target, stack empty.");
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

static JSValue setTransformBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(!rendererInitalized(ctx)) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto array = help.getArray();
    if(help.hasError) return JS_EXCEPTION;
    ObjectHelp objHelp(ctx, array);
    // JS_GetProperty()
    auto length = objHelp.getNumber("length");
    if(length != 16){
        JS_ThrowRangeError(ctx, "Transform matrices must have sixteen elements!");
        return JS_EXCEPTION;
    }
    float data[16];
    for (size_t idx = 0; idx < length; idx++)
    {
        data[idx] = (float)objHelp.getNumber(std::to_string(idx).c_str());
    }
    if(objHelp.hasError){
        JS_ThrowTypeError(ctx, "Error creating transform array.");
        return JS_EXCEPTION;
    }
    auto transform = glm::make_mat4(data);
    engine.renderer.setTransform(transform);
    return JS_UNDEFINED;
}

static JSValue getTransformBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    auto trans = engine.renderer.getTransform();
    auto data = glm::value_ptr(trans);
    auto array = JS_NewArray(ctx);
    ObjectHelp objHelp(ctx, array);
    objHelp.setNumber("length", 16);
    for (size_t idx = 0; idx < 16; idx++)
    {
        objHelp.setNumber(std::to_string(idx).c_str(), data[idx]);
    }
    return array;
}
static JSValue pushTransformBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    engine.renderer.pushTransform();
    return JS_UNDEFINED;
}
static JSValue popTransformBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    engine.renderer.popTransform();
    return JS_UNDEFINED;
}
static JSValue translateBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    float x = help.getFloat64();
    float y = help.getFloat64();
    float z = 0.0f;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.translate(glm::vec3(x, y, z));
    return JS_UNDEFINED;
}
static JSValue scaleBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    float x = help.getFloat64();
    float y = help.getFloat64();
    float z = 0.0f;
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.scale(glm::vec3(x, y, z));
    return JS_UNDEFINED;
}
static JSValue rotateBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto angle = help.getFloat64();
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
    return JS_UNDEFINED;
}
static JSValue setIdentityBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    engine.renderer.setIdentity();
    return JS_UNDEFINED;
}
static JSValue saveTextureBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    FnHelp help(ctx, argc, argv);
    auto path = help.getString();
    int id = -1;
    if(help.hasArgs()){
        auto s = getTexture(help.next());
        if(!s) return JS_EXCEPTION;
        id = s->id;
    }
    if(help.hasError) return JS_EXCEPTION;
    engine.renderer.saveTexture(path.c_str(), id);
    return JS_UNDEFINED;
}


void bindGraphics(){
    JSValue proto, textureProto, shaderProto, meshProto;
    JSValue fn;
    auto ctx = engine.vm.context;
    proto = JS_NewObject(ctx);

    // create Texture class
    /* the class ID is created once */
    JS_NewClassID(&jsTextureClassId);
    /* the class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), jsTextureClassId, &jsTextureClassDef);
    textureProto = JS_NewObject(ctx);
    // constructor(width, height)
    // not working, getting 'not a constructor' error for some reason
    // fn = JS_NewCFunction(ctx, &textureConstructorBind, "textureConstructor", 0);
    // JS_SetConstructorBit(ctx, fn, true);
    // JS_SetConstructor(ctx, fn, textureProto);
    // JS_FreeValue(ctx, fn);

    // static new(with: number, height: number, data: ArrayBuffer)
    fn = JS_NewCFunction(ctx, &textureConstructorBind, "textureConstructor", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "new", fn, 0);
    // static fromArray(with: number, height: number, data: number[])
    fn = JS_NewCFunction(ctx, &textureFromArrayBind, "fromArray", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "fromArray", fn, 0);
    // static fromColor(width: number, height: number, red: number, green: number, blue: number, alpha: number)
    fn = JS_NewCFunction(ctx, &textureFromColorBind, "fromColor", 0);
    JS_DefinePropertyValueStr(ctx, textureProto, "fromColor", fn, 0);
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
    // setting the class prototype consumes the prototype object so we need to reacquire it
    JS_SetClassProto(ctx, jsTextureClassId, textureProto);
    textureProto = JS_GetClassProto(ctx, jsTextureClassId);
    JS_DefinePropertyValueStr(ctx, proto, "Texture", textureProto, 0);

    // Create shader class
    JS_NewClassID(&jsShaderClassId);
    JS_NewClass(JS_GetRuntime(ctx), jsShaderClassId, &jsShaderClassDef);
    shaderProto = JS_NewObject(ctx);
    // static new(vertexSource: string, fragmentSource: string): Shader
    fn = JS_NewCFunction(ctx, &shaderConstructorBind, "shaderConstructor", 0);
    JS_DefinePropertyValueStr(ctx, shaderProto, "new", fn, 0);
    // getUniformLocation(name: string): number
    fn = JS_NewCFunction(ctx, &shaderGetUniformLocationBind, "shaderGetUniformLocation", 0);
    JS_DefinePropertyValueStr(ctx, shaderProto, "getUniformLocation", fn, 0);
    // getAttribLocation(name: string): number
    fn = JS_NewCFunction(ctx, &shaderGetAttribLocationBind, "shaderGetAttribLocation", 0);
    JS_DefinePropertyValueStr(ctx, shaderProto, "getAttribLocation", fn, 0);
    // setting the class prototype consumes the prototype object so we need to reacquire it
    JS_SetClassProto(ctx, jsShaderClassId, shaderProto);
    shaderProto = JS_GetClassProto(ctx, jsShaderClassId);
    JS_DefinePropertyValueStr(ctx, proto, "Shader", shaderProto, 0);

    // Create mesh class
    JS_NewClassID(&jsMeshClassId);
    JS_NewClass(JS_GetRuntime(ctx), jsMeshClassId, &jsMeshClassDef);
    meshProto = JS_NewObject(ctx);
    //static new(shader: Shader, vertexCount: number, vertexSize: number, attributes: [number,string][], data: number[]);
    fn = JS_NewCFunction(ctx, &meshConstructorBind, "meshConstructor", 0);
    JS_DefinePropertyValueStr(ctx, meshProto, "new", fn, 0);

    JS_SetClassProto(ctx, jsMeshClassId, meshProto);
    meshProto = JS_GetClassProto(ctx, jsMeshClassId);
    JS_DefinePropertyValueStr(ctx, proto, "Mesh", meshProto, 0);

    // standalone functions
    // setClearColor(r: number, g: number, b: number, a: number = 0): void
    fn = JS_NewCFunction(ctx, &setClearColorBind, "setClearColor", 0);
    JS_DefinePropertyValueStr(ctx, proto, "setClearColor", fn, 0);
    // clear(): void
    fn = JS_NewCFunction(ctx, &clearBind, "clear", 0);
    JS_DefinePropertyValueStr(ctx, proto, "clear", fn, 0);
    // pushRenderTarget(fb: Texture): void
    fn = JS_NewCFunction(ctx, &pushRenderTargetBind, "pushRenderTarget", 0);
    JS_DefinePropertyValueStr(ctx, proto, "pushRenderTarget", fn, 0);
    // popRenderTarget(): void
    fn = JS_NewCFunction(ctx, &popRenderTargetBind, "popRenderTarget", 0);
    JS_DefinePropertyValueStr(ctx, proto, "popRenderTarget", fn, 0);
    // getTransform(): void
    fn = JS_NewCFunction(ctx, &getTransformBind, "getTransform", 0);
    JS_DefinePropertyValueStr(ctx, proto, "getTransform", fn, 0);
    // setTransform(): void
    fn = JS_NewCFunction(ctx, &setTransformBind, "setTransform", 0);
    JS_DefinePropertyValueStr(ctx, proto, "setTransform", fn, 0);
    // pushTransform(): void
    fn = JS_NewCFunction(ctx, &pushTransformBind, "pushTransform", 0);
    JS_DefinePropertyValueStr(ctx, proto, "pushTransform", fn, 0);
    // popTransform(): void
    fn = JS_NewCFunction(ctx, &popTransformBind, "popTransform", 0);
    JS_DefinePropertyValueStr(ctx, proto, "popTransform", fn, 0);
    // translate(x: number, y: number): void
    fn = JS_NewCFunction(ctx, &translateBind, "translate", 0);
    JS_DefinePropertyValueStr(ctx, proto, "translate", fn, 0);
    // scale(x: number, y: number): void
    fn = JS_NewCFunction(ctx, &scaleBind, "scale", 0);
    JS_DefinePropertyValueStr(ctx, proto, "scale", fn, 0);
    // rotate(x: number, y: number): void
    fn = JS_NewCFunction(ctx, &rotateBind, "rotate", 0);
    JS_DefinePropertyValueStr(ctx, proto, "rotate", fn, 0);
    // setIdentity(): void
    fn = JS_NewCFunction(ctx, &setIdentityBind, "setIdentity", 0);
    JS_DefinePropertyValueStr(ctx, proto, "setIdentity", fn, 0);
    // saveTexture(path: string, texture?: Texture): void
    fn = JS_NewCFunction(ctx, &saveTextureBind, "saveTexture", 0);
    JS_DefinePropertyValueStr(ctx, proto, "saveTexture", fn, 0);
    engine.vm.addExport("Graphics", proto);
}