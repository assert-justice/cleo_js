#pragma once
#include "quickjspp.hpp"

class VM{
    public:
    qjs::Runtime* publicRuntime;
    qjs::Context* publicContext;
    VM();
    ~VM();
    void init(bool* hasError);

    private:
    bool initialized = false;
};
