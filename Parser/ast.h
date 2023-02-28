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
    AST(string type, string attribute, int lineNum);
    AST &setChildren(vector<AST> children);
    AST &addChild(AST child);
    int getKidsLength(AST *self);

    string type;
    string attribute;
    int lineNum;
    vector<AST> kids;
};

#endif