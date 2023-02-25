#include <stdio.h>
#include <iostream>
#include <iomanip>

#include "./scanner/scanner.h"
#include "./scanner/token.h"
#include "./utility/utility.h"

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
    Scanner scanner;
    Token token;
    while ((token = scanner.lex(inputFile, util)).attribute != "EOF")
    {
        cout << setw(10) << token.type << "     [" << token.attribute << "] @ line " << token.lineNum << endl;
    }

    // Close input file
    util.closeInputFile(inputFile);

    return 0;
}