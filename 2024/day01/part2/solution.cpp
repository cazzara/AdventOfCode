#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>


#include "utils.h"


void readInput(std::string filename, std::unordered_map<int, int>& mapA, std::unordered_map<int, int>& mapB)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    while (std::getline(inp, line))
    {
        std::stringstream lineStream {line};
        int a, b;
        lineStream >> a;
        lineStream >> b;
        mapA[a]++;
        mapB[b]++;
    }
}

int calculateSimilarity(std::unordered_map<int, int>& mapA, std::unordered_map<int, int>& mapB)
{
    int similarity = 0;
    for (const auto& pair : mapA)
    {
        int key = pair.first;
        int timesInMapA = pair.second;
        int timesInMapB = mapB[key];
        int score = timesInMapA * timesInMapB * key;
        similarity += score;
    }
    return similarity;
}

int main()
{
    std::unordered_map<int, int> mapA;
    std::unordered_map<int, int> mapB;
    std::string filename = "../test_input.txt";
    readInput(filename, mapA, mapB);
    int similarity = calculateSimilarity(mapA, mapB);
    return 0;
}