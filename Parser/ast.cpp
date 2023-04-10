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

AST::AST(string t) : kids(), sym(nullptr)
{
    type = t;
    attribute = "";
    lineNum = -1;
}

AST::AST(string t, int l) : kids(), sym(nullptr)
{
    type = t;
    lineNum = l;
}

AST::AST(string t, string a) : kids(), sym(nullptr)
{
    type = t;
    attribute = a;
    lineNum = -1;
}

AST::AST(string t, string a, int l) : kids(), sym(nullptr)
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

const AST &AST::operator[](int i) const
{
    return kids[i];
}

void AST::printAST(const AST &ast, int indent)
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
            // Substring not found
            if (!ast.sig.empty())
            {
                before += " sig=";
                before += ast.sig;
                before += " ";
            }
            if (ast.sym != nullptr)
            {
                cout << before << "sym=" << ast.sym << after;
            }
            else
            {
                cout << before << after;
            }

            if (ast.lineNum >= 0)
            {
                cout << " @ line " << ast.lineNum;
            }

            cout << endl;
        }
        else
        {
            // Substring not found
            if (!ast.sig.empty())
            {
                str += " sig=";
                str += ast.sig;
                str += " ";
            }
            // Substring not found
            if (ast.sym != nullptr)
            {
                cout << str << " sym=" << ast.sym;
            }
            else
            {
                cout << str;
            }

            if (ast.lineNum >= 0)
            {
                cout << " @ line " << ast.lineNum;
            }

            cout << endl;
        }

        for (const AST &child : ast.kids)
        {
            printAST(child, indent + 4);
        }
    }
}

void AST::prune() const
{
    throw ASTPrune();
}

void AST::preorder(std::function<void(AST *)> callback)
{
    prepost(callback, [](AST *) {});
}

void AST::postorder(std::function<void(AST *)> callback)
{
    for (AST &kid : kids)
        kid.postorder(callback);
    callback(this);
}

void AST::prepost(std::function<void(AST *)> pre, std::function<void(AST *)> post)
{
    try
    {
        pre(this);
    }
    catch (const ASTPrune &)
    {
        return;
    }

    for (AST &kid : kids)
        kid.prepost(pre, post);

    post(this);
}