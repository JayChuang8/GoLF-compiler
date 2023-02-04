#include "token.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;


Token::Token()
{
}

Token::Token(string t, string a, int l)
{
    type = t;
    attribute = a;
    lineNum = l;
}