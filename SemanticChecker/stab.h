#ifndef stab_h
#define stab_h

#include "../Utility/utility.h"

#include <string>
#include <unordered_map>

struct Symbol
{
    string sig;
};

class SymbolTable
{
public:
    SymbolTable(Utility &util);
    void define(string name, string sig, int lineNum);
    Symbol *lookup(string name, int lineNum);

private:
    std::unordered_map<std::string, Symbol> stab;
    Utility &util;
};

#endif
