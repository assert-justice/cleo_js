#include "bind_audio.hpp"
#include "engine/engine.hpp"
#include "fn_help.hpp"
#include "object_help.hpp"
#include <iostream>

typedef struct {
    int id;
    ma_sound* sound;
} JSSoundClass;

static JSClassID jsSoundClassId;

static void jsSoundClassFinalizer(JSRuntime *rt, JSValue val)
{
    JSSoundClass* s = (JSSoundClass*)JS_GetOpaque(val, jsSoundClassId);
    if (s) {
        if(engine.audio.initalized)
            engine.audio.soundFree(s->id);
        js_free_rt(rt, s);
    }
}

static JSClassDef jsSoundClassDef = {
    "Sound",
    .finalizer = jsSoundClassFinalizer,
}; 

static JSValue newJSSoundHandle(JSContext *ctx, int id, ma_sound* sound)
{
    JSSoundClass* s;
    JSValue obj;
    obj = JS_NewObjectClass(ctx, jsSoundClassId);
    if (JS_IsException(obj))
        return obj;
    s = (JSSoundClass*)js_mallocz(ctx, sizeof(*s));
    if (!s) {
        JS_FreeValue(ctx, obj);
        return JS_EXCEPTION;
    }
    s->id = id;
    s->sound = sound;
    JS_SetOpaque(obj, s);
    return obj;
}

static JSSoundClass* getSound(JSValue val){
    auto s = (JSSoundClass*)JS_GetOpaque(val, jsSoundClassId);
    if(!s){
        JS_ThrowReferenceError(engine.vm.context, "invalid sound handle!");
        return nullptr;
    }
    return s;
}

static JSValue audioInitalized(JSContext* ctx){
    if(!engine.audio.initalized){
        JS_ThrowReferenceError(ctx, "method referenced before initialization!");
        return JS_EXCEPTION;
    }
    return JS_UNDEFINED;
}

static JSValue soundFromFileBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    FnHelp help(ctx, argc, argv);
    auto path = help.getString();
    if(help.hasError) return JS_EXCEPTION;
    auto soundId = engine.audio.soundLoad(path.c_str());
    if(soundId == -1){
        JS_ThrowReferenceError(ctx, "unable to load sound at path!");
        return JS_EXCEPTION;
    }
    auto soundPtr = engine.audio.soundStore.get(soundId);
    auto res = newJSSoundHandle(ctx, soundId, soundPtr);
    return res;
}
static JSValue soundPlayBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    engine.audio.soundPlay(s->id);
    return JS_UNDEFINED;
}
static JSValue soundPauseBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    engine.audio.soundPause(s->id);
    return JS_UNDEFINED;
}
static JSValue soundStopBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    engine.audio.soundStop(s->id);
    return JS_UNDEFINED;
}
static JSValue soundIsPlayingBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewBool(ctx, engine.audio.soundIsPlaying(s->id));
}
static JSValue soundSetVolumeBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    FnHelp fnHelp(ctx, argc, argv);
    auto val = fnHelp.getFloat64();
    if(fnHelp.hasError) return JS_EXCEPTION;
    engine.audio.soundSetVolume(s->id, val);
    return JS_UNDEFINED;
}
static JSValue soundGetVolumeBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewFloat64(ctx, engine.audio.soundGetVolume(s->id));
}
static JSValue soundSetIsLoopingBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    FnHelp fnHelp(ctx, argc, argv);
    auto val = fnHelp.getBool();
    if(fnHelp.hasError) return JS_EXCEPTION;
    engine.audio.soundLoop(s->id, val);
    return JS_UNDEFINED;
}
static JSValue soundGetIsLoopingBind(JSContext* ctx, JSValue thisVal, int argc, JSValue* argv){
    if(JS_IsException(audioInitalized(ctx))) return JS_EXCEPTION;
    auto s = getSound(thisVal);
    if(!s) return JS_EXCEPTION;
    return JS_NewBool(ctx, engine.audio.soundIsLooping(s->id));
}

void bindAudio(){
    JSValue proto, soundProto;
    JSValue fn, fn2;
    auto ctx = engine.vm.context;
    proto = JS_NewObject(ctx);

    // create Sound class
    /* the class ID is created once */
    JS_NewClassID(&jsSoundClassId);
    /* the class is created once per runtime */
    JS_NewClass(JS_GetRuntime(ctx), jsSoundClassId, &jsSoundClassDef);
    soundProto = JS_NewObject(ctx);
    // static fromFile(path: string)
    fn = JS_NewCFunction(ctx, &soundFromFileBind, "fromFile", 0);
    JS_DefinePropertyValueStr(ctx, soundProto, "fromFile", fn, 0);
    // play()
    fn = JS_NewCFunction(ctx, &soundPlayBind, "soundPlay", 0);
    JS_DefinePropertyValueStr(ctx, soundProto, "play", fn, 0);
    // pause()
    fn = JS_NewCFunction(ctx, &soundPauseBind, "soundPause", 0);
    JS_DefinePropertyValueStr(ctx, soundProto, "pause", fn, 0);
    // stop()
    fn = JS_NewCFunction(ctx, &soundStopBind, "soundStop", 0);
    JS_DefinePropertyValueStr(ctx, soundProto, "stop", fn, 0);
    // get isPlaying: boolean
    fn = JS_NewCFunction(ctx, &soundIsPlayingBind, "soundIsPlaying", 0);
    JS_DefineProperty(ctx, soundProto, JS_NewAtom(ctx, "isPlaying"), 
        JS_UNDEFINED, fn, JS_UNDEFINED, JS_PROP_HAS_GET);
    JS_FreeValue(ctx, fn);
    // get set volume: number
    fn = JS_NewCFunction(ctx, &soundGetVolumeBind, "soundGetVolume", 0);
    fn2 = JS_NewCFunction(ctx, &soundSetVolumeBind, "soundSetVolume", 0);
    JS_DefinePropertyGetSet(ctx, soundProto, JS_NewAtom(ctx, "volume"), 
        fn, fn2, 0);
    // get set volume: number
    fn = JS_NewCFunction(ctx, &soundGetIsLoopingBind, "soundGetIsLooping", 0);
    fn2 = JS_NewCFunction(ctx, &soundSetIsLoopingBind, "soundSetVolume", 0);
    JS_DefinePropertyGetSet(ctx, soundProto, JS_NewAtom(ctx, "isLooping"), 
        fn, fn2, 0);

    // setting the class prototype consumes the prototype object so we need to reacquire it
    JS_SetClassProto(ctx, jsSoundClassId, soundProto);
    soundProto = JS_GetClassProto(ctx, jsSoundClassId);
    JS_DefinePropertyValueStr(ctx, proto, "Sound", soundProto, 0);
    engine.vm.addExport("Audio", proto);
}