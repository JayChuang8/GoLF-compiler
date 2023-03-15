#ifndef scanner_h
#define scanner_h

#include "token.h"
#include "../Utility/utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <set>
using namespace std;

class Scanner
{
public:
    Scanner(fstream &inputFile, Utility &util);
    int lineNum;
    Token lastToken;
    bool reread;
    bool insertSemicolon;
    Token lex();
    void unlex();

private:
    Utility &util;
    fstream &inputFile;

    int getLineNum();
    void incrementLineNum();
    Token getLastToken();
    void setLastToken(Token t);
    bool getReread();
    void setReread(bool r);
    bool getInsertSemicolon();
    void setInsertSemicolon(bool s);
};

#endif