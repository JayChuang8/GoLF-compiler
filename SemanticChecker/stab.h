#ifndef stab_h
#define stab_h

#include "../Utility/utility.h"

#include <string>
#include <unordered_map>

struct Symbol
{
    string name;
    string sig;
    string rtname;
    bool isconst;
    bool istype;
};

class SymbolTable
{
public:
    SymbolTable(Utility &util);
    Symbol *define(string name, int lineNum);
    Symbol *define(string name, string sig, int lineNum);
    Symbol *lookup(string name, int lineNum);
    void openScope();
    void closeScope();
    void openSigScope();
    void printScope();
    std::unordered_map<std::string, Symbol> *sigscope;

private:
    Utility &util;
};

#endif
