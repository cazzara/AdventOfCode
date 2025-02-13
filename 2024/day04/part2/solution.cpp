#include <iostream>
#include <fstream>
#include <vector>

#include "utils.h"

void buildInput(std::string filename, std::vector<std::string>& strArray)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    while (std::getline(inp, line))
    {
        strArray.push_back(line);
    }
}

bool checkString(std::string sub, std::string target)
{
    std::string reverseTarget(target.rbegin(), target.rend());
    if (sub.compare(target) == 0 || sub.compare(reverseTarget) == 0)
        return true;
    return false;
}

int findX(std::vector<std::string>& arr, std::string target)
{
    int count = 0;
    int rowIdx = 1;
    while (rowIdx < arr.size() - 1)
    {
        int colIdx = 1;
        while (colIdx < arr[0].size() - 1)
        {
            std::string leftXArm = "";
            leftXArm.push_back(arr[rowIdx - 1][colIdx - 1]);
            leftXArm.push_back(arr[rowIdx][colIdx]);
            leftXArm.push_back(arr[rowIdx + 1][colIdx + 1]);

            std::string rightXArm = "";
            rightXArm.push_back(arr[rowIdx + 1][colIdx - 1]);
            rightXArm.push_back(arr[rowIdx ][colIdx]);
            rightXArm.push_back(arr[rowIdx - 1][colIdx + 1]);

            if (checkString(leftXArm, target) && checkString(rightXArm, target))
                count++;
            colIdx++;
        }
        rowIdx++;
    }
    return count;
}

int main()
{
    std::string filename = "../test_input.txt";
    std::string targetWord = "MAS";
    std::vector<std::string> strArray;
    buildInput(filename, strArray);

    int numX = findX(strArray, targetWord);
    std::cout << "Total Target Word: " << (numX) << std::endl;
    return 0;
}