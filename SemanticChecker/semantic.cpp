#include "semantic.h"

#include "stab.h"
#include "../Utility/utility.h"
#include "../Parser/ast.h"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>

using namespace std;

Semantic::Semantic(Utility &util, SymbolTable &stab) : util(util), stab(stab)
{
}

void Semantic::pass1_cb(AST *node)
{
    if (node->type == "globvar")
    {
        node->sym = stab.define(node->kids[0].attribute, node->kids[0].lineNum);
    }
    else if (node->type == "func")
    {
        node->sym = stab.define(node->kids[0].attribute, node->kids[0].lineNum);
    }
}

void Semantic::pass2_cb(AST *node)
{
    if (node->type == "globvar")
    {
        Symbol *typeSym = stab.lookup(node->kids[1].attribute, node->lineNum);

        if (!typeSym->istype)
        {
            util.error("expected type, got \"" + node->kids[1].attribute + "\"", node->lineNum);
        }
        node->sym->sig = node->kids[1].attribute;
    }
    else if (node->type == "func")
    {
        // set "sig" node sig to func name
        node->kids[1].sig = node->sym->name;
        // set "formals" node sig to func name
        node->kids[1].kids[0].sig = node->sym->name;
    }
    else if (node->type == "sig")
    {
        // open sig scope to indicate we are in a sig block
        stab.openSigScope();

        // set func Symbol's return type
        Symbol *funcSym = stab.lookup(node->sig, node->lineNum);
        funcSym->rettype = node->kids[1].attribute;
    }
    else if (node->type == "formals")
    {
        Symbol *funcSym = stab.lookup(node->sig, node->lineNum);
        string funcSig = "f(";

        // if "formals" has a child, then the function has parameters
        if (node->getKidsLength(node) > 0)
        {
            // create the func sig by iterating through each "formal"
            for (AST child : node->kids)
            {
                funcSig += child.kids[1].attribute;
                funcSig += " ";
            }
            if (funcSig.back() == ' ')
            {
                funcSig.pop_back();
            }
        }
        funcSig += ")";
        funcSym->sig = funcSig;
    }
    else if (node->type == "formal")
    {
        node->kids[0].sym = stab.define(node->kids[0].attribute, node->kids[1].attribute, node->kids[0].lineNum);
    }
    else if (node->type == "var")
    {
        node->sym = stab.define(node->kids[0].attribute, node->kids[1].attribute, node->kids[0].lineNum);
    }
    else if (node->type == "typeid")
    {
        node->sym = stab.lookup(node->attribute, node->lineNum);
    }
    else if (node->type == "id")
    {
        node->sym = stab.lookup(node->attribute, node->lineNum);
    }
    else if (node->type == "block")
    {
        stab.openScope();
    }
}

void Semantic::pass2_post_cb(AST *node)
{
    if (node->type == "block")
    {
        stab.closeScope();
    }
    else if (node->type == "sig" || node->type == "formals")
    {
        // reset sig to empty string
        node->sig = "";
    }
    else if (node->type == "formal")
    {
        if (!node->kids[1].sym->istype)
        {
            util.error("expected type, got \"" + node->kids[1].attribute + "\"", node->kids[1].lineNum);
        }
    }
}

unordered_map<string, vector<vector<string>>> ALLOWED = {
    {"||", {{"bool", "bool", "bool"}}},
    {"&&", {{"bool", "bool", "bool"}}},
    {"==", {{"bool", "bool", "bool"}, {"int", "int", "bool"}, {"string", "string", "bool"}}},
    {"!=", {{"bool", "bool", "bool"}, {"int", "int", "bool"}, {"string", "string", "bool"}}},
    {"=", {{"int", "int", ""}, {"bool", "bool", ""}, {"string", "string", ""}}},
    {"<", {{"int", "int", "bool"}, {"string", "string", "bool"}}},
    {">", {{"int", "int", "bool"}, {"string", "string", "bool"}}},
    {"<=", {{"int", "int", "bool"}, {"string", "string", "bool"}}},
    {">=", {{"int", "int", "bool"}, {"string", "string", "bool"}}},
    {"+", {{"int", "int", "int"}}},
    {"-", {{"int", "int", "int"}}},
    {"*", {{"int", "int", "int"}}},
    {"/", {{"int", "int", "int"}}},
    {"%", {{"int", "int", "int"}}},
    {"u!", {{"bool", "bool"}}},
    {"u-", {{"int", "int"}}}};

unordered_map<string, string> DEFAULTSIGS = {
    {"||", "bool"},
    {"&&", "bool"},
    {"==", "bool"},
    {"!=", "bool"},
    {"=", ""},
    {"<", "bool"},
    {">", "bool"},
    {"<=", "bool"},
    {">=", "bool"},
    {"+", "int"},
    {"-", "int"},
    {"*", "int"},
    {"/", "int"},
    {"%", "int"},
    {"u!", "bool"},
    {"u-", "int"}};

bool Semantic::typecheckok(AST *node)
{
    // Build list of child node type signatures
    vector<string> siglist;
    for (int i = 0; i < node->getKidsLength(node); i++)
    {
        siglist.push_back(node->kids[i].sig);
        // cout << node->kids[i].sig << endl;
    }

    // Iterate over allowed signatures for node type
    for (vector<string> ok : ALLOWED[node->type])
    {
        if (node->getKidsLength(node) + 1 != ok.size())
        {
            // Wrong arity
            continue;
        }
        if (vector<string>(ok.begin(), ok.end() - 1) == siglist)
        {
            // change sig later
            node->sig = ok.back(); // Set result type
            return true;
        }
    }

    return false;
}

void Semantic::pass3_cb(AST *node)
{
    if (node->type == "id")
    {
        node->sig = node->sym->sig;
    }
    else if (node->type == "int")
    {
        node->sig = "int";
    }
    else if (node->type == "string")
    {
        node->sig = "string";
    }
    else if (node->type == "bool")
    {
        node->sig = "bool";
    }
    else if (node->type == "if" || node->type == "ifelse" || node->type == "for")
    {
        if (node->kids[0].sig != "bool")
        {
            util.error(node->type + " expression must be boolean type", node->kids[0].lineNum);
        }
    }
    else if (node->type == "funccall")
    {
        if (node->kids[0].type != "id")
        {
            util.error("can't call something that isn't a function", node->kids[0].lineNum);
        }

        node->sig = node->kids[0].sym->rettype;

        Symbol *funcSym = stab.lookup(node->kids[0].attribute, node->lineNum);
        string funcSig = "f(";

        // if "actuals" has a child/children, then the function call has parameters
        if (node->getKidsLength(&(node->kids[1])) > 0)
        {
            // create the func sig by iterating through the "actuals" children
            for (AST child : node->kids[1].kids)
            {
                funcSig += child.sig;
                funcSig += " ";
            }
            if (funcSig.back() == ' ')
            {
                funcSig.pop_back();
            }
        }

        funcSig += ")";
        if (funcSym->sig != funcSig)
        {
            util.error("number/type of arguments doesn't match function declaration", node->lineNum);
        }
    }
    else if (ALLOWED.count(node->type))
    {
        node->sig = DEFAULTSIGS.at(node->type);

        if (!typecheckok(node))
        {
            util.error("operand type mismatch for \"" + node->type + "\"", node->lineNum);
        }
    }
}

int forlevel = 0;
int returncount = 0;

void Semantic::pass4_cb(AST *node)
{
    if (node->type == "=")
    {
        if (node->kids[0].sym != nullptr && node->kids[0].sym->isconst)
        {
            util.error("can't assign to a constant", node->lineNum);
        }

        // double check node->kids[0].type != "id", may need to include "var"???
        if (node->kids[0].sym == nullptr || node->kids[0].type != "id")
        {
            util.error("can only assign to a variable", node->lineNum);
        }
    }
    else if (node->type == "int")
    {
        try
        {
            int num = std::stoi(node->attribute);

            if (num > std::numeric_limits<int>::max())
            {
                util.error("integer literal too large", node->lineNum);
            }
            if (num < std::numeric_limits<int>::min())
            {
                util.error("integer literal too large", node->lineNum);
            }
        }
        catch (std::out_of_range &ex)
        {
            util.error("integer literal out of range", node->lineNum);
        }
    }
    else if (node->type == "for")
    {
        forlevel++;
    }
    else if (node->type == "break")
    {
        if (forlevel == 0)
        {
            util.error("break must be inside 'for'", node->lineNum);
        }
    }
    else if (node->type == "return")
    {
        returncount++;
    }
    else if (node->type == "id")
    {
        if (node->sym->istype)
        {
            util.error("can't use type \"" + node->attribute + "\" here", node->lineNum);
        }
    }
}

void Semantic::pass4_post_cb(AST *node)
{
    if (node->type == "program")
    {
        stab.lookup("main", node->lineNum);
    }
    else if (node->type == "func")
    {
        if (node->sym->name == "main" && node->sym->sig != "f()")
        {
            util.error("main() can't have arguments", node->lineNum);
        }

        if (node->sym->name == "main" && node->sym->rettype != "$void")
        {
            util.error("main() can't have a return value", node->lineNum);
        }

        if (node->sym->rettype != "$void" && returncount == 0)
        {
            util.error("no return statement in function", node->lineNum);
        }

        if (node->sym->rettype == "$void" && returncount != 0)
        {
            util.error("this function can't return a value", node->kids[2].kids[0].lineNum);
        }

        for (AST child1 : node->kids)
        {
            if (child1.type == "block")
            {
                for (AST child2 : child1.kids)
                {
                    if (child2.type == "return" &&
                        child2.getKidsLength(&child2) == 1 &&
                        node->sym->rettype != child2.kids[0].sig)
                    {
                        util.error("returned value has the wrong type", child2.kids[0].lineNum);
                    }
                }
            }
        }

        if (node->sym->rettype != "$void" &&
            node->kids.size() > 2 &&
            node->kids[2].kids.size() > 0 &&
            node->kids[2].kids[0].kids.size() == 0)
        {
            util.error("this function must return a value", node->kids[2].kids[0].lineNum);
        }

        if (returncount != 0)
        {
            returncount = 0;
        }
    }
    else if (node->type == "for")
    {
        forlevel--;
    }
}

std::unordered_map<string, int> names; // map names to declaration locations

void Semantic::semantic(AST &ast)
{
    // Pass 1 (pre):
    // Gather up all top level file scope names - global var declarations, function declaration names
    // But not insert info about their types (basically placeholders)
    // create file scope
    stab.openScope();
    ast.preorder([this](AST *node)
                 { pass1_cb(node); });

    // Pass 2 (pre/post):
    // Fully define global names
    // figure out what all the identifiers do, link id nodes to proper symbol table entries
    ast.prepost(
        [this](AST *node)
        { pass2_cb(node); },
        [this](AST *node)
        { pass2_post_cb(node); });

    // pass 3 (post): do most of the type checking
    // propagate info from bottom up which will correspond to a summary of the type of that subtree
    // will know symbol that an identifier will correspond to from previous pass
    //
    // An if-, if-else, or for-condition must be of Boolean type
    // Type mismatch for an operator (||, &&, ==, !=, =, <, >, <=, >=, +, - (unary and binary), *, /, %, !)
    // The number/type of arguments in a function call doesn't match the function's declaration
    ast.postorder([this](AST *node)
                  { pass3_cb(node); });

    // pass 4 (pre/post)
    // check assignments, verify not assigning to a const or non variable
    // check break statement in for loop
    // check return statement is encountered in inappropriate context
    // check if integer is within range
    // check if main() has arguments
    ast.prepost(
        [this](AST *node)
        { pass4_cb(node); },
        [this](AST *node)
        { pass4_post_cb(node); });

    for (auto iter = names.begin(); iter != names.end(); ++iter)
    {
        const string &name = iter->first;
        int lineNum = iter->second;
        util.warning("unused variable \"" + name + "\"", lineNum);
    }
}