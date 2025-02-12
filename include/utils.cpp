#include <iostream>
#include <fstream>
#include "utils.h"

std::ifstream openFile(std::string filename)
{
    std::ifstream inp{filename};
    if (!inp.is_open())
    {
        throw std::runtime_error("unable to open file: " + filename);
    }
    return inp;
}

int charToInt(char c)
{
    return c - '0';
}
