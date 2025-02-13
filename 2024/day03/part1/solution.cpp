#include <fstream>
#include <iostream>
#include <regex>

#include "utils.h"

std::string readInput(std::string filename)
{
    std::ifstream inp = openFile(filename);
    std::string content((std::istreambuf_iterator<char>(inp)), std::istreambuf_iterator<char>());
    return content;
}

void processInput(std::string input)
{
    std::regex regex("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    int x, y;
    int sum = 0;
    std::string tempStr = input;
    for (std::smatch sm; std::regex_search(tempStr, sm, regex);)
    {
        x = std::stoi(sm[1].str());
        y = std::stoi(sm[2].str());
        sum += (x * y);
        tempStr = sm.suffix();
    }
    std::cout << "The sum is: " << sum;
}

int main()
{
    std::string filename = "../input.txt";
    std::string puzzleInput = readInput(filename);
    processInput(puzzleInput);
    return 0;
}