#include "scanner.h"
#include "token.h"
#include "../Utility/utility.h"

#include <ctype.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <set>
#include <unordered_set>
using namespace std;

const unordered_set<string> keywords = {"break", "else", "for", "func", "if", "return", "var"};

const unordered_set<string> semicolonTokens = {
    "id", "int", "string", "break", "return", ")", "}"};

const unordered_set<char> escapeChars = {'b', 'f', 'n', 'r', 't', '\\', '"'};

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
            if (c == '\x20' || c == '\x09' || c == '\x0D' || c == '\x0A')
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
                char nextChar = inputFile.peek();
                if (nextChar == '/')
                {
                    while (!inputFile.eof() && c != '\n')
                    {
                        inputFile.get(c);
                    }
                    inputFile.putback(c);
                    continue;
                }
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

            // string literals
            case '"':
            {
                inputFile.get(c);
                string str = "";
                while (((c >= 0 && c <= 127) || c == '"') && !inputFile.eof() && c != '\n')
                {
                    if (c == '\\')
                    {
                        char nextChar = inputFile.peek();
                        if (escapeChars.count(nextChar) <= 0)
                        {
                            inputFile.get(c);
                            string message = "bad string escape '";
                            message += c;
                            message += '\'';

                            if (inputFile.eof())
                            {
                                message = "string terminated by EOF";
                            }
                            else if (c == '\n')
                            {
                                message = "string contains newline";
                            }
                            util.error(message, getLineNum());
                        }
                        // if there is \" escape_char
                        else if (nextChar == '"')
                        {
                            // add backslash to string
                            str += c;
                            inputFile.get(c);
                        }
                    }
                    else if (c == '"')
                    {
                        break;
                    }

                    str += c;
                    inputFile.get(c);
                }

                if (inputFile.eof())
                {
                    util.error("string terminated by EOF", getLineNum());
                }
                else if (c == '\n')
                {
                    util.error("string contains newline", getLineNum());
                }
                else if (c == '"')
                {
                    setLastToken(Token("string", str, getLineNum()));
                }
                break;
            }

            default:
            {
                // integer literals
                if (isdigit(c))
                {
                    string num = "";
                    while (isdigit(c) && !inputFile.eof())
                    {
                        num += c;
                        inputFile.get(c);
                    }
                    setLastToken(Token("int", num, getLineNum()));

                    // If the token is the last in the file, don't putback the EOF
                    if (!inputFile.eof())
                    {
                        inputFile.putback(c);
                    }
                }
                // identifier and keywords
                else if (isalpha(c) || c == '_')
                {
                    string id = "";
                    while ((isalpha(c) || isdigit(c) || c == '_') && !inputFile.eof())
                    {
                        id += c;
                        inputFile.get(c);
                    }

                    // if id is a reserved keyword
                    if (keywords.count(id) > 0)
                    {
                        setLastToken(Token(id, id, getLineNum()));
                    }
                    else
                    {
                        setLastToken(Token("id", id, getLineNum()));
                    }

                    // If the token is the last in the file, don't putback the EOF
                    if (!inputFile.eof())
                    {
                        inputFile.putback(c);
                    }
                }
                else
                {
                    string message = "skipping unknown character '";
                    message += c;
                    message += '\'';

                    if (c == '\x00')
                    {
                        message = "skipping NUL character";
                    }
                    else if (c < 0 || c > 127)
                    {
                        message = "skipping non-ASCII input character";
                    }
                    util.warning(message, getLineNum());
                    continue;
                }
            }
            }

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