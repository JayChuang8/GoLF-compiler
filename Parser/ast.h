#ifndef ast_h
#define ast_h

#include <iostream>
#include <stdio.h>
#include <vector>
#include <vector>
#include <functional>
#include <stdexcept>
#include <string>
#include "../SemanticChecker/stab.h"
using namespace std;

class AST
{
public:
    class ASTPrune : public std::exception
    {
    };

    AST();
    AST(string type);
    AST(string type, int lineNum);
    AST(string type, string attribute);
    AST(string type, string attribute, int lineNum);
    AST &addChild(AST child);
    AST &setChildren(vector<AST> children);
    int getKidsLength(AST *self);
    const AST &operator[](int i) const;
    void printAST(const AST &ast, int indent);
    void prune() const;
    void preorder(std::function<void(AST *)> callback);
    void postorder(std::function<void(AST *)> callback);
    void prepost(std::function<void(AST *)> pre, std::function<void(AST *)> post);

    int countChildrenByType(AST &node, string type);

    string type;
    string attribute;
    int lineNum;
    string sig;
    string reg;
    Symbol *sym;
    vector<AST> kids;
};

#endif