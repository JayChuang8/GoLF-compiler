#ifndef token_h
#define token_h

#include <iostream>
#include <stdio.h>
using namespace std;


class Token
{
    public:
        Token();
        Token(string type, string attribute, int linNum);

        string type;
        string attribute;
        int lineNum;
};

#endif