#ifndef parser_h
#define parser_h

#include "ast.h"
#include "../Scanner/token.h"
#include "../Scanner/scanner.h"
#include "../Utility/utility.h"

#include <iostream>
#include <stdio.h>
using namespace std;

class Parser
{
public:
    Parser(Scanner &scanner, Utility &util);
    AST parse();

    string type;
    string attribute;
    int lineNum;

private:
    Token expect(string type);

    AST Arguments();
    AST Assignment();
    AST BasicLit();
    AST Block();
    AST BreakStmt();
    AST Condition();
    AST Declaration();
    AST EmptyStmt();
    AST Expression();
    AST ExpressionList();
    AST ExpressionRest();
    AST ExpressionStmt();
    AST ForStmt();
    AST FunctionBody();
    AST FunctionDecl();
    AST FunctionName();
    AST IfStmt();
    AST Literal();
    AST Operand();
    AST OperandName();
    AST ParameterDecl();
    AST ParameterList();
    AST Parameters();
    AST PrimaryExpr();
    AST PrimaryExprRest();
    AST Result();
    AST ReturnStmt();
    AST Signature();
    AST SimpleStmt();
    AST SourceFile();
    AST Statement();
    AST StatementList();
    AST TopLevelDecl();
    AST Type();
    AST TypeName();
    AST UnaryExpr();
    AST VarDecl();
    AST VarSpec();

    Scanner &scanner;
    Utility &util;
};

#endif