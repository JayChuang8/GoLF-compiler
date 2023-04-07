#ifndef stab_h
#define stab_h

#include "../Utility/utility.h"

#include <string>
#include <unordered_map>

struct Symbol
{
    string sig;
    string rtname;
    bool isconst;
    bool istype;
};

class SymbolTable
{
public:
    SymbolTable(Utility &util);
    Symbol *define(string name, string sig, int lineNum);
    Symbol *lookup(string name, int lineNum);
    void openScope();
    void closeScope();

private:
    Utility &util;
};

#endif
