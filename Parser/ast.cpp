#include "ast.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

AST::AST()
{
}

AST::AST(string t) : kids()
{
    type = t;
    attribute = "";
    lineNum = 0;
}

AST::AST(string t, int l) : kids()
{
    type = t;
    lineNum = l;
}

AST::AST(string t, string a, int l) : kids()
{
    type = t;
    attribute = a;
    lineNum = l;
}

AST &AST::setChildren(vector<AST> children)
{
    kids = move(children);
    return *this;
}

AST &AST::addChild(AST child)
{
    kids.push_back(child);
    return *this;
}

int AST::getKidsLength(AST *self)
{
    return self->kids.size();
}

void AST::printAST(const AST &ast, int indent = 0)
{
    for (int i = 0; i < indent; i++)
    {
        cout << " ";
    }

    cout << ast.type << " [" << ast.attribute << "] " << ast.lineNum << endl;

    for (const AST &child : ast.kids)
    {
        printAST(child, indent + 4);
    }
}