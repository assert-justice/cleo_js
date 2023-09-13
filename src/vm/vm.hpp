#pragma once
#include <string>
#include "quickjs.h"

class VM{
    public:
    JSRuntime* runtime;
    JSContext* context;
    JSValue initFn = JS_UNDEFINED;
    JSValue updateFn = JS_UNDEFINED;
    ~VM();
    void init(bool* hasError);
    void bind(bool* hasError, std::string src);
    void launch();
    void update(double dt);

    private:
    bool initialized = false;
};

bool isException(JSContext* ctx, JSValue val);

class FnHelp{
    public:
    bool hasError = false;
    FnHelp(JSContext* ctx, int argc, JSValue* argv);
    JSValue next();
    std::string getString();
    double getFloat64();
    int getInt();
    JSValue getFunction();
    bool hasArgs();
    private:
    JSContext* ctx;
    int argc;
    JSValue* argv;
    std::string name;
    int argIdx = 0;
};