#pragma once
#include <string>
#include "quickjs.h"

class VM{
    public:
    JSRuntime* runtime;
    JSContext* context;
    JSValue initFn;
    JSValue loopFn;
    ~VM();
    void init(bool* hasError);
    void bind(std::string src);
    void launch();

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
    JSValue getFunction();
    private:
    JSContext* ctx;
    int argc;
    JSValue* argv;
    std::string name;
    int argIdx = 0;
};