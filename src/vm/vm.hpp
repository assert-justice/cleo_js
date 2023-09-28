#pragma once
#include <string>
#include <vector>
#include "quickjs.h"

class VM{
    public:
    JSRuntime* runtime;
    JSContext* context;
    JSValue initFn = JS_UNDEFINED;
    JSValue updateFn = JS_UNDEFINED;
    JSValue drawFn = JS_UNDEFINED;
    ~VM();
    void init(bool* hasError);
    void bind(bool* hasError, std::string src);
    void launch(bool* hasError);
    void update(double dt);
    void draw();
    void addExport(std::string name, JSValue value);
    std::vector<std::pair<std::string, JSValue>> exports;

    private:
    bool initialized = false;
};

bool isException(JSContext* ctx, JSValue val);