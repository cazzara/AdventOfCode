#include <fstream>
#include <iostream>
#include <sstream>

#include "utils.h"

int checkReportSafety(std::vector<int>& levels)
{
    int num;
    int prev;
    bool decreasing;
    bool increasing;
    for (int i = 1; i < levels.size(); i++)
    {
        prev = levels[i - 1];
        num = levels[i];
        if (num > prev)
            increasing = true;
        else if (num < prev)
            decreasing = true;
        if (increasing && decreasing)
            return i;
        int magnitude = std::abs(num - prev);
        if (magnitude < 1 || magnitude > 3)
            return i;
    }
    return -1;
}

std::vector<int> copyVector(std::vector<int>& vec, int idx)
{
    std::vector<int> copy;
    for (int i = 0; i < vec.size(); i++)
    {
        if (i != idx)
            copy.push_back(vec[i]);
    }
    return copy;
}

void processInputFile(std::string filename)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int safeReports = 0;
    while (std::getline(inp, line))
    {
        std::stringstream report {line};
        std::vector<int> levels;
        int level;
        while (report >> level)
            levels.push_back(level);
        int idx = checkReportSafety(levels);
        if (idx == -1)
            safeReports++;
        else
        {
            for (int i = 0; i < levels.size(); i++)
            {
                std::vector<int> copy = copyVector(levels, i);
                if (checkReportSafety(copy) == -1)
                {
                    safeReports++;
                    break;
                }
            }
        }
    }
    std::cout << "Number of safe reports: " << safeReports << std::endl;
}


int main()
{
    std::string filename = "../input.txt";
    processInputFile(filename);
    return 0;
}