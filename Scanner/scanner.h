#ifndef scanner_h
#define scanner_h

#include "token.h"
#include "../Utility/utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;


class Scanner
{
    public:
        Scanner();

        int lineNum;
        Token lastToken;
        bool reread;

        Token lex(fstream& inputFile);
        void unlex();

    private:
        int getLineNum();
        void incrementLineNum();
        Token getLastToken();
        void setLastToken(Token t);
        bool getReread();
        void setReread(bool r);
};

#endif