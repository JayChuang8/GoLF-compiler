#include <stdio.h>
#include <iostream>

#include "./scanner/scanner.h"
#include "./scanner/token.h"


int main(int argc, char *argv[])
{
    // Check command line input for correct number of arguments
    if (argc != 2)
    {
        std::cerr << "Error: Requires 1 input file" << "\n";
        exit(0);
    }

    // Scanner


    std::cout << "You have entered " << argc
         << " arguments:" << "\n";
  
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";

    return 0;
}