#include "scanner.h"
#include "token.h"
#include "../Utility/utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

Scanner::Scanner()
{
    lineNum = 1;
}

void Scanner::unlex()
{
    this->reread = true;
}

Token Scanner::lex(fstream &inputFile, Utility &util)
{
    char c;

    if (getReread() == true)
    {
        setReread(false);
        return getLastToken();
    }

    if (inputFile.is_open())
    {
        while (true)
        {
            inputFile.get(c);

            // EOF
            if (inputFile.eof())
            {
                setLastToken(Token("EOF", "EOF", getLineNum()));
                return getLastToken();
            }

            // whitespace
            if (isspace(c))
            {
                if (c == '\n')
                {
                    incrementLineNum();
                }
                continue;
            }

            switch (c)
            {
            // comments
            case '/':
            {
                while (!inputFile.eof() && c != '\n')
                {
                    inputFile.get(c);
                }
                inputFile.putback(c);
                continue;
            }

            // singletons
            case '+':
            case '-':
            case '*':
            case '%':
            case '(':
            case ')':
            case '{':
            case '}':
            case ',':
            case ';':
            {
                setLastToken(Token(string(1, c), string(1, c), getLineNum()));
                break;
            }

            // two-char tokens
            case '&':
            {
                inputFile.get(c);
                if (c != '&')
                {
                    util.error("bitwise AND not supported in GoLF, expected '&&' at or near line", getLineNum());
                }
                else
                {
                    setLastToken(Token("&&", "&&", getLineNum()));
                }
                break;
            }

            case '|':
            {
                inputFile.get(c);
                if (c != '|')
                {
                    util.error("bitwise OR not supported in GoLF, expected '||' at or near line", getLineNum());
                }
                else
                {
                    setLastToken(Token("||", "||", getLineNum()));
                }
                break;
            }

            case '=':
            {
                inputFile.get(c);
                if (c == '=')
                {
                    // replace with util error method later
                    setLastToken(Token("==", "==", getLineNum()));
                }
                else
                {
                    inputFile.putback(c);
                    setLastToken(Token("=", "=", getLineNum()));
                }
            }

            case '<':
            {
                inputFile.get(c);
                if (c == '=')
                {
                    // replace with util error method later
                    setLastToken(Token("<=", "<=", getLineNum()));
                }
                else
                {
                    inputFile.putback(c);
                    setLastToken(Token("<", "<", getLineNum()));
                }
                break;
            }

            case '>':
            {
                inputFile.get(c);
                if (c == '=')
                {
                    // replace with util error method later
                    setLastToken(Token(">=", ">=", getLineNum()));
                }
                else
                {
                    inputFile.putback(c);
                    setLastToken(Token(">", ">", getLineNum()));
                }
                break;
            }

            case '!':
            {
                inputFile.get(c);
                if (c == '=')
                {
                    // replace with util error method later
                    setLastToken(Token("!=", "!=", getLineNum()));
                }
                else
                {
                    inputFile.putback(c);
                    setLastToken(Token("!", "!", getLineNum()));
                }
                break;
            }

            default:
            {
                util.warning("unknown character", getLineNum());
                continue;
            }
            }

            // cout << getLastToken().type << " " << getLastToken().attribute << " " << getLastToken().lineNum << endl;
            return getLastToken();
        }
    }
    return getLastToken();
}

int Scanner::getLineNum()
{
    return lineNum;
}

void Scanner::incrementLineNum()
{
    lineNum++;
}

Token Scanner::getLastToken()
{
    return lastToken;
}

void Scanner::setLastToken(Token t)
{
    lastToken = t;
}

bool Scanner::getReread()
{
    return reread;
}

void Scanner::setReread(bool r)
{
    reread = r;
}