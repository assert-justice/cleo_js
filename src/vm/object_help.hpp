#pragma once
#include "quickjs.h"
#include <string>

class ObjectHelp{
    public:
    bool hasError = false;
    ObjectHelp(JSContext* ctx, JSValue obj);
    JSValue getVal(const char* name);
    JSValue getValRequired(const char* name);
    double getNumber(const char* name, double defaultVal = 0);
    void setNumber(const char* name, double val);
    bool getBool(const char* name, bool defaultVal = false);
    void setBool(const char* name, bool val);
    std::string getString(const char* name);
    void setString(const char* name, const char* string);
    private:
    JSContext* ctx;
    JSValue obj;
};