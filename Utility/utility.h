#ifndef utility_h
#define utility_h

#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

class Utility
{
private:
    Utility(){};
    Utility(Utility const &copy);
    Utility &operator=(Utility const &copy);

public:
    static Utility &getInstance();

    int numWarnings;

    void openInputFile(fstream &inputFile, const char *fileName);
    void closeInputFile(fstream &inputFile);
    void error(string message, int lineNum);
    void warning(string message, int lineNum);
};

#endif
