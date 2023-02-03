#ifndef utility_h
#define utility_h

#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;


class Utility
{
    private:
        Utility() {};
        Utility(Utility const& copy);
        Utility& operator=(Utility const& copy);

    public:
        static Utility& getInstance();

        fstream inputFile;
        int numWarnings;

        void openInputFile(const char *fileName);
        void closeInputFile();
        void error(string message, int lineNum);
        void warning(string message, int lineNum);
};

#endif

