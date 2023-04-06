#ifndef stab_h
#define stab_h

#include "../Utility/utility.h"

#include <string>
#include <unordered_map>

class Symbol
{
public:
    string sig;

    Symbol();
    Symbol(const string &s);
};

class SymbolTable
{
private:
    std::unordered_map<string, Symbol> stab;
    Utility &util;

public:
    SymbolTable(Utility &util);
    void define(const string &name, const string &sig, int lineNum);
    Symbol &lookup(const string &name, int lineNum);
};

#endif
