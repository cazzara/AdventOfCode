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
    return (sub.compare(target) == 0 || sub.compare(reverseTarget) == 0);
}

int findHorizontal(std::vector<std::string>& arr, std::string target)
{
    int count = 0;
    int rowIdx = 0;
    while (rowIdx < arr.size())
    {
        int colIdx = 0;
        while (colIdx <= arr[0].size() - target.size())
        {
            std::string sub = arr[rowIdx].substr(colIdx, target.size());
            if (checkString(sub, target))
                count++;
            colIdx++;
        }
        rowIdx++;
    }
    return count;
}

int findVertical(std::vector<std::string>& arr, std::string target)
{
    int count = 0;
    int colIdx = 0;
    while(colIdx < arr[0].size())
    {
        int rowIdx = 0;
        while (rowIdx <= arr.size() - target.size())
        {
            std::string sub = "";
            for (int i = 0; i < target.size(); i++)
                sub.push_back(arr[rowIdx + i][colIdx]);
            if (checkString(sub, target))
                count++;
            rowIdx++;
        }
        colIdx++;
    }
    return count;
}

int findDiagonal(std::vector<std::string>& arr, std::string target)
{
    int count = 0;
    int rowIdx = 0;
    while (rowIdx <= arr.size() - target.size())
    {
        int colIdx = 0;
        while (colIdx <= arr[0].size())
        {
            std::string sub = "";
            for (int i = 0; i < target.size(); i++)
                sub.push_back(arr[rowIdx + i][colIdx + i]);
            if (checkString(sub, target))
                count++;
            colIdx++;
        }
        rowIdx++;
    }
    rowIdx = 0;
    while (rowIdx <= arr.size() - target.size())
    {
        int colIdx = arr[0].size() - 1;
        while (colIdx >= target.size() - 1)
        {
            std::string sub = "";
            for (int i = 0; i < target.size(); i++)
                sub.push_back(arr[rowIdx + i][colIdx - i]);
            if (checkString(sub, target))
                count++;
            colIdx--;
        }
        rowIdx++;
    }
    return count;
}

int main()
{
    std::string filename = "../input.txt";
    std::string targetWord = "XMAS";
    std::vector<std::string> strArray;
    buildInput(filename, strArray);

    int numHorizontal = findHorizontal(strArray, targetWord);
    int numVertical = findVertical(strArray, targetWord);
    int numDiagonal = findDiagonal(strArray, targetWord);
    std::cout << "Total Target Word: " << (numHorizontal + numVertical + numDiagonal) << std::endl;
    return 0;
}