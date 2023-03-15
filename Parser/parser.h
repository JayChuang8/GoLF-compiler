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

private:
    AST AddOpExpr();
    AST AndOpExpr();
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
    AST ExpressionStmt();
    AST ForStmt();
    AST FunctionBody();
    AST FunctionDecl();
    AST FunctionName();
    AST IfStmt();
    AST Literal();
    AST MulOpExpr();
    AST Operand();
    AST OperandName();
    AST OrOpExpr();
    AST ParameterDecl();
    AST ParameterList();
    AST Parameters();
    AST PrimaryExpr();
    AST RelOpExpr();
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
    Token expect(string type);
    Utility &util;
};

#endif