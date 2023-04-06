#include "stab.h"
#include "../Utility/utility.h"

#include <iostream>

using namespace std;

Symbol::Symbol() : sig("") {}
Symbol::Symbol(const string &s) : sig(s) {}

SymbolTable::SymbolTable(Utility &util) : util(util)
{
}

void SymbolTable::define(const string &name, const string &sig, int lineNum)
{
    if (stab.count(name) > 0)
    {
        util.error("\"" + name + "\" redefined", lineNum);
    }
    stab[name] = Symbol(sig);
}

Symbol &SymbolTable::lookup(const string &name, int lineNum)
{
    if (stab.count(name) > 0)
    {
        return stab[name];
    }
    util.error("unknown identifier \"" + name + "\"", lineNum);
    static Symbol s("");
    return s;
}
