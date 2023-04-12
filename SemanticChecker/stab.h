#ifndef stab_h
#define stab_h

#include "../Utility/utility.h"

#include <string>
#include <unordered_map>

struct Symbol
{
    string name;
    string sig;
    string rettype;
    string rtname;
    bool isconst;
    bool istype;
    int allocspace;
    string reg;
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
