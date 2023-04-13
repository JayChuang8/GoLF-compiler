#include "utility.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#define MaxWarnings 10

Utility &Utility::getInstance()
{
    static Utility instance;
    return instance;
}

void Utility::openInputFile(fstream &inputFile, const char *fileName)
{
    inputFile.open(fileName, ios::in);
    if (!inputFile)
    {
        cerr << "error: file '" << fileName << "' could not be opened" << endl;
        exit(1);
    }
}

void Utility::closeInputFile(fstream &inputFile)
{
    inputFile.close();
}

void Utility::error(string message, int lineNum = 0)
{
    string errorMessage;

    if (lineNum == 0)
    {
        errorMessage = message;
    }
    else
    {
        errorMessage = message + " at or near line " + to_string(lineNum);
    }

    cerr << "error: " << errorMessage << endl;
    exit(1);
}

void Utility::warning(string message, int lineNum)
{
    cerr << "warning: " << message << " at or near line " << lineNum << endl;

    Utility::numWarnings++;

    if (numWarnings > MaxWarnings)
    {
        error("Too many warnings", lineNum);
    }
}