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