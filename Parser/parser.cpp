#include "parser.h"
#include "ast.h"
#include "../Scanner/token.h"
#include "../Scanner/scanner.h"
#include "../Utility/utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_set>
using namespace std;

const unordered_set<string> ARGUMENTS = {"("};
const unordered_set<string> ASSIGNMENT = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> BASICLIT = {"int", "string"};
const unordered_set<string> BINARYOP = {"||", "&&", "==", "!=", "<", "<=", ">", ">=", "+", "-", "*", "/", "%"};
const unordered_set<string> BLOCK = {"{"};
const unordered_set<string> BREAKSTMT = {"break"};
const unordered_set<string> CONDITION = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> DECLARATION = {"var"};
const unordered_set<string> EMPTYSTMT = {";"};
const unordered_set<string> EXPRESSION = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> EXPRESSIONLIST = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> EXPRESSIONSTMT = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> FORSTMT = {"for"};
const unordered_set<string> FUNCTIONDECL = {"func"};
const unordered_set<string> IFSTMT = {"if"};
const unordered_set<string> LITERAL = {"int", "string"};
const unordered_set<string> OPERAND = {"int", "string", "id", "("};
const unordered_set<string> OPERANDNAME = {"id"};
const unordered_set<string> PARAMETERLIST = {"id"};
const unordered_set<string> PARAMETERS = {"("};
const unordered_set<string> PRIMARYEXPR = {"int", "string", "id", "("};
const unordered_set<string> RESULT = {"id"};
const unordered_set<string> RETURNSTMT = {"return"};
const unordered_set<string> SIMPLESTMT = {";", "-", "!", "int", "string", "id", "("};
const unordered_set<string> STATEMENT = {"var", ";", "-", "!", "int", "string", "id", "(", "return", "break", "if", "{", "for"};
const unordered_set<string> TOPLEVELDECL = {"var", "func"};
const unordered_set<string> UNARYOP = {"-", "!"};
const unordered_set<string> VARDECL = {"var"};

Parser::Parser(Scanner &scanner, Utility &util) : scanner(scanner), util(util)
{
}

Token Parser::expect(string type)
{
    Token token = scanner.lex();
    if (token.type != type)
    {
        string message = "unexpected " + token.type + ", expected " + type;
        util.error(message, token.lineNum);
    }
    return token;
}

AST Parser::Arguments()
{
    Token token = expect("(");
    AST ast = AST("Arguments", token.attribute, token.lineNum);

    if ((token = scanner.lex()).type == ")")
    {
        // AST child(token.type, token.attribute, token.lineNum);
        // Dont want to add ")" to children?
        // ast.addChild(child);
    }
    else if (EXPRESSIONLIST.count(token.type) > 0)
    {
        scanner.unlex();

        AST child(ExpressionList());
        ast.addChild(child);

        if ((token = scanner.lex()).type == ",")
        {
            // child = AST(token.type, token.attribute, token.lineNum);
            // Dont want to add "," to children?
            // ast.addChild(child);
        }
        else
        {
            scanner.unlex();
        }

        token = expect(")");
        // child = AST(token.type, token.attribute, token.lineNum);
        // Dont want to add ")" to children?
        // ast.addChild(child);
    }
    else
    {
        util.error("expected Arguments", token.lineNum);
    }
    return ast;
}

AST Parser::Assignment()
{
    AST ast = Expression();
    expect("=");
    AST child(Expression());
    ast.addChild(child);
    return ast;
}

AST Parser::BasicLit()
{
    Token token = scanner.lex();
    AST ast;

    if (BASICLIT.count(token.type) > 0)
    {
        ast = AST(token.type, token.attribute, token.lineNum);
    }
    else
    {
        util.error("expected BasicLit", token.lineNum);
    }
    return ast;
}

AST Parser::Block()
{
    expect("{");
    AST ast = StatementList();
    expect("}");
    return ast;
}

AST Parser::BreakStmt()
{
    Token token = expect("break");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    return ast;
}

AST Parser::Condition()
{
    AST ast = Expression();
    return ast;
}

AST Parser::Declaration()
{
    AST ast = VarDecl();
    return ast;
}

AST Parser::EmptyStmt()
{
    Token token = expect(";");
    AST ast = AST("emptystmt", token.attribute, token.lineNum);
    return ast;
}

AST Parser::Expression()
{
    Token token;
    AST L;
    vector<Token> unary_op_tokens;

    while (UNARYOP.count((token = scanner.lex()).type) > 0)
    {
        unary_op_tokens.push_back(token);
    }
    scanner.unlex();

    L = PrimaryExpr();

    while (BINARYOP.count((token = scanner.lex()).type) > 0)
    {
        AST R = Expression();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();

    // Add unary operators to the AST node
    for (int i = 0; i < unary_op_tokens.size(); i++)
    {
        token = unary_op_tokens.back();
        unary_op_tokens.pop_back();
        vector<AST> children = {L};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    return L;
}

AST Parser::ExpressionList()
{
    AST L = Expression();

    Token token;
    while ((token = scanner.lex()).attribute == ",")
    {
        AST R = Expression();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

// AST Parser::ExpressionRest()
// {
//     AST ast;
//     Token token = scanner.lex();

//     if (BINARYOP.count(token.type) > 0)
//     {
//         ast = AST(token.type, token.attribute, token.lineNum);
//         AST child(Expression());
//         ast.addChild(child);
//         child = AST(ExpressionRest());
//         ast.addChild(child);
//     }
//     scanner.unlex();
//     // return empty ast for empty string/epsilon case
//     return ast;
// }

AST Parser::ExpressionStmt()
{
    AST ast = Expression();
    return ast;
}

AST Parser::ForStmt()
{
    Token token = expect("for");
    AST ast = AST(token.type, token.lineNum);

    if (CONDITION.count((token = scanner.lex()).type) > 0)
    {
        scanner.unlex();
        AST child(Condition());
        ast.addChild(child);
    }
    else
    {
        scanner.unlex();
    }

    AST child(Block());
    ast.addChild(child);
    return ast;
}

AST Parser::FunctionBody()
{
    AST ast = Block();
    return ast;
}

AST Parser::FunctionDecl()
{
    Token token = expect("func");
    AST ast = AST(token.type, token.lineNum);
    AST child(FunctionName());
    ast.addChild(child);
    child = AST(Signature());
    ast.addChild(child);
    child = AST(FunctionBody());
    ast.addChild(child);
    return ast;
}

AST Parser::FunctionName()
{
    Token token = expect("id");
    AST ast = AST("newid", token.attribute, token.lineNum);
    return ast;
}

AST Parser::IfStmt()
{
    Token token = expect("if");
    AST ast = AST(token.type, token.lineNum);
    AST child(Expression());
    ast.addChild(child);
    child = AST(Block());
    ast.addChild(child);

    if ((token = scanner.lex()).attribute == "else")
    {
        child = AST(token.type, token.attribute, token.lineNum);
        ast.addChild(child);

        token = scanner.lex();
        if (IFSTMT.count(token.attribute) > 0)
        {
            child = AST(IfStmt());
            ast.addChild(child);
        }
        else if (BLOCK.count(token.attribute) > 0)
        {
            child = AST(Block());
            ast.addChild(child);
        }
    }
    else
    {
        scanner.unlex();
    }
    return ast;
}

AST Parser::Literal()
{
    AST ast = BasicLit();
    return ast;
}

AST Parser::Operand()
{
    Token token = scanner.lex();
    AST ast;

    if (LITERAL.count(token.type) > 0)
    {
        scanner.unlex();
        ast = Literal();
    }
    else if (OPERANDNAME.count(token.type) > 0)
    {
        scanner.unlex();
        ast = OperandName();
    }
    else if (token.attribute == "(")
    {
        // Dont want to add "(" to children?
        // ast = AST(token.type, token.attribute, token.lineNum);
        // AST child(Expression());
        // ast.addChild(child);
        ast = Expression();
        expect(")");
    }
    else
    {
        util.error("expected Operand", token.lineNum);
    }
    return ast;
}

AST Parser::OperandName()
{
    Token token = expect("id");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    return ast;
}

AST Parser::ParameterDecl()
{
    Token token = expect("id");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    token = expect("id");
    AST child(token.type, token.attribute, token.lineNum);
    ast.addChild(child);
    return ast;
}

AST Parser::ParameterList()
{
    AST L = ParameterDecl();

    Token token;
    while ((token = scanner.lex()).type == ",")
    {
        AST R = ParameterDecl();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

AST Parser::Parameters()
{
    Token token = expect("(");
    AST ast = AST("sig");

    if ((token = scanner.lex()).type == ")")
    {
        AST child("formals");
        ast.addChild(child);
    }
    else if (PARAMETERLIST.count(token.type) > 0)
    {
        scanner.unlex();

        AST child(ParameterList());
        ast.addChild(child);

        if ((token = scanner.lex()).type == ",")
        {
            child = AST(token.type, token.attribute, token.lineNum);
            // Dont want to add "," to children?
            // ast.addChild(child);
        }
        else
        {
            scanner.unlex();
        }

        token = expect(")");
        child = AST(token.type, token.attribute, token.lineNum);
        // Dont want to add ")" to children?
        // ast.addChild(child);
    }
    else
    {
        util.error("expected Parameters", token.lineNum);
    }
    return ast;
}

AST Parser::PrimaryExpr()
{
    AST ast = Operand();
    Token token;

    while (ARGUMENTS.count((token = scanner.lex()).type) > 0)
    {
        scanner.unlex();
        AST child(Arguments());
        ast.addChild(child);
    }
    scanner.unlex();
    return ast;
}

// AST Parser::PrimaryExprRest()
// {
//     Token token = scanner.lex();
//     AST ast;

//     if (ARGUMENTS.count(token.attribute) > 0)
//     {
//         scanner.unlex();
//         ast = Arguments();
//         AST child(PrimaryExprRest());
//         ast.addChild(child);
//     }
//     scanner.unlex();
//     // return empty ast for empty string/epsilon case
//     return ast;
// }

AST Parser::Result()
{
    AST ast = Type();
    return ast;
}

AST Parser::ReturnStmt()
{
    Token token = expect("return");
    AST ast = AST(token.type, token.attribute, token.lineNum);

    if (EXPRESSION.count((token = scanner.lex()).type) > 0)
    {
        scanner.unlex();
        AST child(Expression());
        ast.addChild(child);
    }
    else
    {
        scanner.unlex();
    }
    return ast;
}

AST Parser::Signature()
{
    AST ast = Parameters();

    Token token = scanner.lex();
    if (RESULT.count(token.type) > 0)
    {
        scanner.unlex();
        AST child(Result());
        ast.addChild(child);
    }
    else
    {
        scanner.unlex();
    }
    return ast;
}

AST Parser::SimpleStmt()
{
    Token token = scanner.lex();
    AST ast;
    if (EMPTYSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = EmptyStmt();
    }
    else if (EXPRESSIONSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = ExpressionStmt();
    }
    else if (ASSIGNMENT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = Assignment();
    }
    else
    {
        util.error("expected Simple Statement", token.lineNum);
    }
    return ast;
}

AST Parser::SourceFile()
{
    AST ast = AST("program");
    Token token;
    while (TOPLEVELDECL.count((token = scanner.lex()).attribute) > 0)
    {
        scanner.unlex();
        AST child(TopLevelDecl());
        ast.addChild(child);
        expect(";");
    }
    scanner.unlex();
    return ast;
}

AST Parser::Statement()
{
    Token token = scanner.lex();
    AST ast;

    if (DECLARATION.count(token.type) > 0)
    {
        scanner.unlex();
        ast = Declaration();
    }
    else if (SIMPLESTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = SimpleStmt();
    }
    else if (RETURNSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = ReturnStmt();
    }
    else if (BREAKSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = BreakStmt();
    }
    else if (BLOCK.count(token.type) > 0)
    {
        scanner.unlex();
        ast = Block();
    }
    else if (IFSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = IfStmt();
    }
    else if (FORSTMT.count(token.type) > 0)
    {
        scanner.unlex();
        ast = ForStmt();
    }
    else
    {
        util.error("expected Statement", token.lineNum);
    }
    return ast;
}

AST Parser::StatementList()
{
    AST ast = AST("block");
    Token token;
    while (STATEMENT.count((token = scanner.lex()).attribute) > 0 ||
           STATEMENT.count(token.type) > 0)
    {
        scanner.unlex();
        AST child(Statement());
        ast.addChild(child);
        expect(";");
    }
    scanner.unlex();
    return ast;
}

AST Parser::TopLevelDecl()
{
    Token token = scanner.lex();
    AST ast;

    if (DECLARATION.count(token.type) > 0)
    {
        scanner.unlex();
        ast = Declaration();
    }
    else if (FUNCTIONDECL.count(token.type) > 0)
    {
        scanner.unlex();
        ast = FunctionDecl();
    }
    else
    {
        util.error("expected TopLevelDecl", token.lineNum);
    }
    return ast;
}

AST Parser::Type()
{
    AST ast = TypeName();
    return ast;
}

AST Parser::TypeName()
{
    Token token = expect("id");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    return ast;
}

AST Parser::UnaryExpr()
{
    Token token = scanner.lex();
    AST ast;

    if (PRIMARYEXPR.count(token.type) > 0)
    {
        scanner.unlex();
        ast = PrimaryExpr();
    }
    else if (UNARYOP.count(token.type) > 0)
    {
        ast = AST(token.type, token.attribute, token.lineNum);
        AST child(UnaryExpr());
        ast.addChild(child);
    }
    else
    {
        util.error("expected UnaryExpr", token.lineNum);
    }
    return ast;
}

AST Parser::VarDecl()
{
    Token token = expect("var");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    AST child(VarSpec());
    ast.addChild(child);
    return ast;
}

AST Parser::VarSpec()
{
    Token token = expect("id");
    AST ast = AST(token.type, token.attribute, token.lineNum);
    AST child(Type());
    ast.addChild(child);
    return ast;
}

AST Parser::parse()
{
    AST ast = SourceFile();
    Token token;
    if ((token = scanner.lex()).attribute != "EOF")
    {
        util.error("Expected source file", token.lineNum);
    }
    return ast;
}