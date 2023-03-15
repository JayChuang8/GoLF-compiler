#ifndef ast_h
#define ast_h

#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;

class AST
{
public:
    AST();
    AST(string type);
    AST(string type, int lineNum);
    AST(string type, string attribute);
    AST(string type, string attribute, int lineNum);
    AST &addChild(AST child);
    AST &setChildren(vector<AST> children);
    int getKidsLength(AST *self);
    void printAST(const AST &ast, int indent);

    string type;
    string attribute;
    int lineNum;
    vector<AST> kids;
};

#endif