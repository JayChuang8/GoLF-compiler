#include "stab.h"
#include "../Utility/utility.h"

#include <iostream>

using namespace std;

SymbolTable::SymbolTable(Utility &util) : util(util)
{
}

void SymbolTable::define(string name, string sig, int lineNum)
{
    if (stab.count(name) > 0)
    {
        util.error("\"" + name + "\" redefined", lineNum);
    }
    Symbol s{sig};
    stab[name] = s;
}

Symbol &SymbolTable::lookup(string name, int lineNum)
{
    if (stab.count(name) > 0)
    {
        return stab[name];
    }
    util.error("unknown identifier \"" + name + "\"", lineNum);
    Symbol s{""};
    return s;
}
