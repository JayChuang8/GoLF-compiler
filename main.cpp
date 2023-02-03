#include <stdio.h>
#include <iostream>

#include "./scanner/scanner.h"
#include "./scanner/token.h"
#include "./utility/utility.h"


int main(int argc, char *argv[]) 
{
    // Check command line input for correct number of arguments
    if (argc != 2) {
        std::cerr << "Error: Requires 1 input file" << "\n";
        exit(0);
    }

    // Open input file
    Utility& util = Utility::getInstance();
    util.openInputFile(argv[1]);

    // Scanner


    util.closeInputFile();


    // if (util.inputFile.is_open()) { //checking whether the file is open
    //     string tp;
    //     while(getline(util.inputFile, tp)){ //read data from file object and put it into string.
    //         cout << tp << "\n"; //print the data of the string
    //     }
    //     util.inputFile.close(); //close the file object.
    // }

    // std::cout << "You have entered " << argc
    //      << " arguments:" << "\n";
  
    // for (int i = 0; i < argc; ++i)
    //     std::cout << argv[i] << "\n";

    return 0;
}