#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "./Scanner/scanner.h"
#include "./Scanner/token.h"
#include "./Utility/utility.h"
#include "./Parser/parser.h"
#include "./Parser/ast.h"
#include "./SemanticChecker/semantic.h"
#include "./SemanticChecker/stab.h"
#include "./Generator/backend_asm.h"

int main(int argc, char *argv[])
{
    // Check command line input for correct number of arguments
    if (argc != 2)
    {
        std::cerr << "Error: Requires 1 input file"
                  << "\n";
        exit(0);
    }

    // Open input file
    Utility &util = Utility::getInstance();
    fstream inputFile;
    util.openInputFile(inputFile, argv[1]);

    // Scanner
    // Scanner scanner(inputFile, util);
    // Token token;
    // while ((token = scanner.lex()).attribute != "EOF")
    // {
    //     cout << setw(10) << token.type << "     [" << token.attribute << "] @ line " << token.lineNum << endl;
    // }

    // // Parser
    Scanner scanner(inputFile, util);
    Parser parser(scanner, util);
    AST ast = parser.parse();

    // // Semantic checker
    SymbolTable stab(util);
    Semantic semantic(util, stab);
    semantic.semantic(ast);

    // // Print ast
    ast.printAST(ast, 0);

    // // MIPS code generator
    // BackendASM generator(util);
    // generator.gen(ast);

    // Close input file
    util.closeInputFile(inputFile);

    return 0;
}