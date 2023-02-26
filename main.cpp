#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "./scanner/scanner.h"
#include "./scanner/token.h"
#include "./utility/utility.h"
#include "./parser/parser.h"
#include "./parser/ast.h"

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
    Scanner scanner(inputFile, util);
    Token token;
    while ((token = scanner.lex()).attribute != "EOF")
    {
        cout << setw(10) << token.type << "     [" << token.attribute << "] @ line " << token.lineNum << endl;
    }

    // Parser
    // Scanner scanner(inputFile, util);
    // Parser parser;
    // AST ast = parser.parse(scanner);

    // Close input file
    util.closeInputFile(inputFile);

    return 0;
}