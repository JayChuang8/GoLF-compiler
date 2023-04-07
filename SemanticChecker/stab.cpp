#include "stab.h"
#include "../Utility/utility.h"

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

std::vector<std::unordered_map<std::string, Symbol>> stabstack;

SymbolTable::SymbolTable(Utility &util) : util(util)
{
    // initialize the UNIVERSE symbol table and add it to the symbol table stack
    std::unordered_map<std::string, Symbol> UNIVERSE;

    UNIVERSE.insert({"$void", {"void", "", false, true}});
    UNIVERSE.insert({"bool", {"bool", "", false, true}});
    UNIVERSE.insert({"$int", {"int", "", false, true}});
    UNIVERSE.insert({"string", {"str", "", false, true}});
    UNIVERSE.insert({"$true", {"bool", "Ltrue", true, false}});
    UNIVERSE.insert({"true", {"bool", "Ltrue", true, false}});
    UNIVERSE.insert({"false", {"bool", "Lfalse", true, false}});
    UNIVERSE.insert({"printb", {"f(bool) void", "Lprintb", false, false}});
    UNIVERSE.insert({"printc", {"f(int) void", "Lprintc", false, false}});
    UNIVERSE.insert({"printi", {"f(int) void", "Lprinti", false, false}});
    UNIVERSE.insert({"prints", {"f(str) void", "Lprints", false, false}});
    UNIVERSE.insert({"getchar", {"f() int", "Lgetchar", false, false}});
    UNIVERSE.insert({"halt", {"f() void", "Lhalt", false, false}});
    UNIVERSE.insert({"len", {"f(str) int", "Llen", false, false}});

    stabstack.push_back(UNIVERSE);
}

Symbol *SymbolTable::define(string name, string sig, int lineNum)
{
    // access current scope symbol table and look for name in the table
    std::unordered_map<std::string, Symbol> &stab = stabstack.back();
    auto iter = stab.find(name);

    if (iter != stab.end())
    {
        // error if the name already exists in the symbol table (redefined in current scope)
        util.error("\"" + name + "\" redefined", lineNum);
    }
    Symbol s{sig};
    stab[name] = s;
    return &stab[name];
}

Symbol *SymbolTable::lookup(string name, int lineNum)
{
    // Iterate through the symbol table stack, starting from the most recent symbol table
    for (auto rit = stabstack.rbegin(); rit != stabstack.rend(); ++rit)
    {
        // Lookup name in the current symbol table
        auto &stab = *rit;
        auto iter = stab.find(name);
        if (iter != stab.end())
        {
            // Return the first symbol table entry found
            return &(iter->second);
        }
    }
    // If the name isn't found in any symbol table, then the id is unknown
    util.error("unknown identifier \"" + name + "\"", lineNum);
    return nullptr;
}

void SymbolTable::openScope()
{
    stabstack.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::closeScope()
{
    stabstack.pop_back();
}
