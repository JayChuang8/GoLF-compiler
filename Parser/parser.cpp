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

const unordered_set<string> ADDOP = {"+", "-"};
const unordered_set<string> ANDOP = {"&&"};
const unordered_set<string> ARGUMENTS = {"("};
const unordered_set<string> ASSIGNOP = {"="};
const unordered_set<string> BASICLIT = {"int", "string"};
const unordered_set<string> BINARYOP = {"||", "&&", "==", "!=", "<", "<=", ">", ">=", "+", "-", "*", "/", "%"};
const unordered_set<string> BLOCK = {"{"};
const unordered_set<string> BREAKSTMT = {"break"};
const unordered_set<string> DECLARATION = {"var"};
const unordered_set<string> ELSESTMT = {"else"};
const unordered_set<string> EMPTYSTMT = {";"};
const unordered_set<string> EXPRESSION = {"-", "!", "int", "string", "id", "("};
const unordered_set<string> FORSTMT = {"for"};
const unordered_set<string> FUNCTIONDECL = {"func"};
const unordered_set<string> IFSTMT = {"if"};
const unordered_set<string> LITERAL = {"int", "string"};
const unordered_set<string> MULOP = {"*", "/", "%"};
const unordered_set<string> OPERAND = {"int", "string", "id", "("};
const unordered_set<string> OPERANDNAME = {"id"};
const unordered_set<string> OROP = {"||"};
const unordered_set<string> PARAMETERLIST = {"id"};
const unordered_set<string> PARAMETERS = {"("};
const unordered_set<string> PRIMARYEXPR = {"int", "string", "id", "("};
const unordered_set<string> RELOP = {"==", "!=", "<", "<=", ">", ">="};
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

AST Parser::AddOpExpr()
{
    AST L = MulOpExpr();

    Token token;
    while (ADDOP.count((token = scanner.lex()).type) > 0)
    {
        AST R = MulOpExpr();
        vector<AST> children = {L, R};
        L = AST(token.type, "AddOpExpr" + token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

AST Parser::AndOpExpr()
{
    AST L = RelOpExpr();

    Token token;
    while (ANDOP.count((token = scanner.lex()).type) > 0)
    {
        AST R = RelOpExpr();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

AST Parser::Arguments()
{
    Token token = expect("(");
    AST ast;

    if ((token = scanner.lex()).type == ")")
    {
        // AST child(token.type, token.attribute, token.lineNum);
        // Dont want to add ")" to children?
        // ast.addChild(child);
    }
    else if (EXPRESSION.count(token.type) > 0)
    {
        scanner.unlex();

        ast = ExpressionList();
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
    // unlex since empty statement does not do anything
    scanner.unlex();
    return ast;
}

AST Parser::Expression()
{
    AST ast = OrOpExpr();
    return ast;
}

AST Parser::ExpressionList()
{
    AST ast = AST("actuals");
    AST child = Expression();
    ast.addChild(child);

    Token token;
    while ((token = scanner.lex()).type == "," && (token = scanner.lex()).type != ")")
    {
        scanner.unlex();
        child = Expression();
        ast.addChild(child);
    }
    scanner.unlex();
    return ast;
}

AST Parser::ExpressionStmt()
{
    AST ast = Expression();
    return ast;
}

AST Parser::ForStmt()
{
    Token token = expect("for");
    AST ast = AST(token.type, token.attribute, token.lineNum);

    if (EXPRESSION.count((token = scanner.lex()).type) > 0)
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
    AST ast = AST("func", token.lineNum);
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

    while (ELSESTMT.count((token = scanner.lex()).attribute) > 0)
    {
        Token elseToken = token;
        if (IFSTMT.count((token = scanner.lex()).attribute) > 0)
        {
            AST ifElse = AST("ifelse", token.lineNum);
            AST child(Expression());
            ifElse.addChild(child);
            child = AST(Block());
            ifElse.addChild(child);
            ast.addChild(ifElse);
        }
        else if (BLOCK.count(token.type) > 0)
        {
            scanner.unlex();
            AST child(Block());
            ast.addChild(child);
            return ast;
        }
        else
        {
            util.error("expected If Statement", token.lineNum);
        }
    }
    scanner.unlex();
    return ast;
}

AST Parser::Literal()
{
    AST ast = BasicLit();
    return ast;
}

AST Parser::MulOpExpr()
{
    AST L = UnaryExpr();

    Token token;
    while (MULOP.count((token = scanner.lex()).type) > 0)
    {
        AST R = UnaryExpr();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
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

AST Parser::OrOpExpr()
{
    AST L = AndOpExpr();

    Token token;
    while (OROP.count((token = scanner.lex()).type) > 0)
    {
        AST R = AndOpExpr();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

AST Parser::ParameterDecl()
{
    Token token = expect("id");
    AST L = AST(token.type, token.attribute, token.lineNum);
    AST R = Type();
    vector<AST> children = {L, R};
    AST ast = AST("formal").setChildren(children);
    return ast;
}

AST Parser::ParameterList()
{
    AST ast = AST("formals");
    AST child = ParameterDecl();
    ast.addChild(child);

    Token token;
    while ((token = scanner.lex()).type == "," && (token = scanner.lex()).type != ")")
    {
        scanner.unlex();
        child = ParameterDecl();
        ast.addChild(child);
    }
    scanner.unlex();
    return ast;
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

        if ((token = scanner.lex()).type != ",")
        {
            scanner.unlex();
        }

        token = expect(")");
    }
    else
    {
        util.error("expected Parameters", token.lineNum);
    }
    return ast;
}

AST Parser::PrimaryExpr()
{
    Token token = scanner.lex();
    AST funcCall = AST("funccall", token.lineNum);
    scanner.unlex();
    AST ast = Operand();

    while (ARGUMENTS.count((token = scanner.lex()).type) > 0)
    {
        ast = funcCall.addChild(ast);
        scanner.unlex();
        AST child = Arguments();
        ast.addChild(child);
    }
    scanner.unlex();
    return ast;
}

AST Parser::RelOpExpr()
{
    AST L = AddOpExpr();

    Token token;
    while (RELOP.count((token = scanner.lex()).type) > 0)
    {
        AST R = AddOpExpr();
        vector<AST> children = {L, R};
        L = AST(token.type, token.attribute, token.lineNum).setChildren(children);
    }
    scanner.unlex();
    return L;
}

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
    else if (EXPRESSION.count(token.type) > 0)
    {
        AST exprtstmt = AST("exprstmt", token.lineNum);

        scanner.unlex();
        ast = Expression();

        token = scanner.lex();
        if (ASSIGNOP.count(token.type) > 0)
        {
            AST R = Expression();
            vector<AST> children = {ast, R};
            ast = AST(token.type, token.attribute, token.lineNum).setChildren(children);
        }
        else
        {
            ast = exprtstmt.addChild(ast);
            scanner.unlex();
        }
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
    AST ast = AST("typeid", token.attribute, token.lineNum);
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
        ast = AST(token.type, "UNARYOP " + token.attribute, token.lineNum);
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
    token = expect("id");
    AST child = AST("newid", token.attribute, token.lineNum);
    ast.addChild(child);
    child = Type();
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