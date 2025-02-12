#include <fstream>
#include <iostream>
#include <sstream>

#include "utils.h"

int checkReportSafety(std::stringstream& report)
{
    int num;
    int prev = INT32_MAX;
    bool decreasing;
    bool increasing;
    while (report >> num)
    {
        if (prev == INT32_MAX)
        {
            prev = num;
            continue;
        }
        if (num > prev)
            increasing = true;
        else if (num < prev)
            decreasing = true;
        if (increasing && decreasing)
            return 0;
        int magnitude = std::abs(num - prev);
        if (magnitude < 1 || magnitude > 3)
            return 0;
        prev = num;
    }
    return 1;
}

void processInputFile(std::string filename)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int safeReports = 0;
    while (std::getline(inp, line))
    {
        std::stringstream report {line};
        safeReports += checkReportSafety(report);
    }
    std::cout << "Number of safe reports: " << safeReports << std::endl;
}


int main()
{
    std::string filename = "../input.txt";
    processInputFile(filename);
    return 0;
}