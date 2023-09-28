#pragma once
#include "quickjs.h"

class ObjectHelp{
    public:
    bool hasError = false;
    ObjectHelp(JSContext* ctx, JSValue obj);
    JSValue getVal(const char* name);
    JSValue getValRequired(const char* name);
    double getNumber(const char* name, double defaultVal = 0);
    void setNumber(const char* name, double val);
    private:
    JSContext* ctx;
    JSValue obj;
};