#include "utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;


const int MAXNUMWARNINGS = 10;

Utility& Utility::getInstance()
{
    static Utility instance;
    return instance;
}

void Utility::openInputFile(const char *fileName)
{
    inputFile.open(fileName, ios::in);
    if(!inputFile) {
        cerr << "Error: file '" << fileName << "' could not be opened" << endl;
        exit(1);
    }
}

void Utility::closeInputFile()
{
	inputFile.close();
}

void Utility::error(string message, int lineNum = 0)
{
    string errorMessage;

    if(lineNum == 0) {
        errorMessage = message;
    } else {
        errorMessage = message + " at or near " + to_string(lineNum);
    }

    cerr << "Error: " << errorMessage << endl;
    exit(1);
}

void Utility::warning(string message, int lineNum)
{
    cerr << "Warning: " << message << " at or near " << lineNum << endl;

    Utility::numWarnings++;

    if(numWarnings > MAXNUMWARNINGS) {
        error("Too many warnings", lineNum);
    }
}