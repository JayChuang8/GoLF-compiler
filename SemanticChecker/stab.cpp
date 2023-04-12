#include "stab.h"
#include "../Utility/utility.h"

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

std::vector<std::unordered_map<std::string, Symbol> *> stabstack;
bool sigscopeAppended = false;

SymbolTable::SymbolTable(Utility &util) : util(util), sigscope(nullptr)
{
    // initialize the UNIVERSE symbol table and add it to the symbol table stack
    std::unordered_map<std::string, Symbol> *UNIVERSE = new std::unordered_map<std::string, Symbol>;

    UNIVERSE->insert({"$void", {"$void", "void", "", "", false, true}});
    UNIVERSE->insert({"bool", {"bool", "bool", "", "", false, true}});
    UNIVERSE->insert({"int", {"int", "int", "", "", false, true}});
    UNIVERSE->insert({"string", {"string", "string", "", "", false, true}});
    UNIVERSE->insert({"$true", {"$true", "bool", "", "Ltrue", true, false}});
    UNIVERSE->insert({"true", {"true", "bool", "", "Ltrue", true, false}});
    UNIVERSE->insert({"false", {"false", "bool", "", "Lfalse", true, false}});
    UNIVERSE->insert({"printb", {"printb", "f(bool)", "void", "Lprintb", false, false}});
    UNIVERSE->insert({"printc", {"printc", "f(int)", "void", "Lprintc", false, false}});
    UNIVERSE->insert({"printi", {"printi", "f(int)", "void", "Lprinti", false, false}});
    UNIVERSE->insert({"prints", {"prints", "f(string)", "void", "Lprints", false, false}});
    UNIVERSE->insert({"getchar", {"getchar", "f()", "int", "Lgetchar", false, false}});
    UNIVERSE->insert({"halt", {"halt", "f()", "void", "Lhalt", false, false}});
    UNIVERSE->insert({"len", {"len", "f(string)", "int", "Llen", false, false}});

    stabstack.push_back(UNIVERSE);
}

Symbol *SymbolTable::define(string name, int lineNum)
{
    // access current scope symbol table and look for name in the table
    std::unordered_map<std::string, Symbol> *stab = stabstack.back();

    auto iter = stab->find(name);

    if (iter != stab->end())
    {
        // error if the name already exists in the symbol table (redefined in current scope)
        util.error("\"" + name + "\" redefined", lineNum);
    }
    Symbol s{name};
    (*stab)[name] = s;
    return &((*stab)[name]);
}

Symbol *SymbolTable::define(string name, string sig, int lineNum)
{
    // access current scope symbol table and look for name in the table
    std::unordered_map<std::string, Symbol> *stab = stabstack.back();
    auto iter = stab->find(name);

    if (iter != stab->end())
    {
        // error if the name already exists in the symbol table (redefined in current scope)
        util.error("\"" + name + "\" redefined", lineNum);
    }
    Symbol s{name, sig};
    (*stab)[name] = s;
    return &((*stab)[name]);
}

Symbol *SymbolTable::lookup(string name, int lineNum)
{
    // Iterate through the symbol table stack, starting from the most recent symbol table
    for (auto rit = stabstack.rbegin(); rit != stabstack.rend(); ++rit)
    {
        // Lookup name in the current symbol table
        auto stab = *rit;
        auto iter = stab->find(name);
        if (iter != stab->end())
        {
            // Return the first symbol table entry found
            return &(iter->second);
        }
    }

    if (name == "main")
    {
        util.error("missing main() function", 0);
    }
    // If the name isn't found in any symbol table, then the id is unknown
    util.error("unknown identifier \"" + name + "\"", lineNum);
    return nullptr;
}

void SymbolTable::openScope()
{
    // if sigscope is a nullptr, that means there is no sig/function parameters to be included in this block
    // if sigscope is pointing to something, that means a block is already open with sig Symbols and we can include them in the block
    // Accounts for: 3. The scope of an identifier denoting a function parameter is the function body.
    if (sigscope == nullptr || sigscopeAppended)
    {
        stabstack.push_back(new std::unordered_map<std::string, Symbol>());
    }
    else
    {
        sigscopeAppended = true;
    }
}

void SymbolTable::closeScope()
{
    // close sig scope if it is open and is the next scope to be popped off
    if (!stabstack.empty())
    {
        if (sigscope == &*stabstack.back())
        {
            sigscope = nullptr;
            sigscopeAppended = false;
        }
    }
    stabstack.pop_back();
}

void SymbolTable::openSigScope()
{
    // Point sigscope to a new scope
    sigscope = new std::unordered_map<std::string, Symbol>();
    stabstack.push_back(sigscope);
}

void SymbolTable::printScope()
{
    for (const auto &stab : stabstack)
    {
        std::cout << "----- STAB -----" << std::endl;
        for (const auto &symbol : *stab)
        {
            std::cout << symbol.first << " : " << symbol.second.sig << " : " << symbol.second.rettype << " : " << symbol.second.rtname << " : " << symbol.second.isconst << " : " << symbol.second.istype << std::endl;
        }
    }
}