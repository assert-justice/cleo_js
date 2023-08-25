#pragma once
#include "quickjs.h"

class VM{
    public:
    JSRuntime* runtime;
    JSContext* context;
    VM();
    ~VM();
    void init(bool* hasError);

    private:
    bool initialized = false;
};
