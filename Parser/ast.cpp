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
    lineNum = -1;
}

AST::AST(string t, int l) : kids()
{
    type = t;
    lineNum = l;
}

AST::AST(string t, string a) : kids()
{
    type = t;
    attribute = a;
    lineNum = -1;
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
    if (!ast.type.empty())
    {
        for (int i = 0; i < indent; i++)
        {
            cout << " ";
        }

        string str = ast.type + " [" + ast.attribute + "]";
        string substring = "[]";

        // Find the position of the substring
        size_t pos = str.find(substring);

        if (pos != string::npos)
        {
            // Extract the part of the string before the substring
            string before = str.substr(0, pos);

            // Extract the part of the string after the substring
            string after = str.substr(pos + substring.length());

            // Print the modified string
            if (ast.lineNum < 0)
            {
                cout << before << after << endl;
            }
            else
            {
                cout << before << after << "@ line " << ast.lineNum << endl;
            }
        }
        else
        {
            // Substring not found
            if (ast.lineNum < 0)
            {
                cout << str << endl;
            }
            else
            {
                cout << str << " @ line " << ast.lineNum << endl;
            }
        }

        for (const AST &child : ast.kids)
        {
            printAST(child, indent + 4);
        }
    }
}