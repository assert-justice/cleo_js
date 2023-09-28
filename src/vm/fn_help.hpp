#pragma once
#include <string>
#include "quickjs.h"

class FnHelp{
    public:
    bool hasError = false;
    FnHelp(JSContext* ctx, int argc, JSValue* argv);
    JSValue next();
    std::string getString();
    double getFloat64();
    int getInt();
    bool getBool();
    JSValue getFunction();
    bool hasArgs();
    private:
    JSContext* ctx;
    int argc;
    JSValue* argv;
    std::string name;
    int argIdx = 0;
};