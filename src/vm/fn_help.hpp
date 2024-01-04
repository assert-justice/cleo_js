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
    JSValue getArray();
    bool hasArgs();
    int getArgIndex();

    private:
    JSContext* ctx;
    int argc;
    JSValue* argv;
    std::string name;
    int argIdx = 0;
};