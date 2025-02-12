#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "utils.h"

void readInput(std::string filename, std::vector<int>& listA, std::vector<int>& listB)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    while(std::getline(inp, line))
    {
        std::stringstream lineStream {line};
        int a,b;
        lineStream >> a;
        lineStream >> b;
        listA.push_back(a);
        listB.push_back(b);
    }
}

int calculateDistance(std::vector<int>& listA, std::vector<int>& listB)
{
    int distance = 0;
    for (int i = 0; i < listA.size(); i++)
    {
        distance += std::abs(listA[i] - listB[i]);
    }
    return distance;

}

int main()
{
    std::vector<int> listA;
    std::vector<int> listB;
    std::string filename{"../input.txt"};
    readInput(filename, listA, listB);
    std::sort(listA.begin(), listA.end());
    std::sort(listB.begin(), listB.end());
    int distance = calculateDistance(listA, listB);
    std::cout << "The distance between the two lists is: " << distance << std::endl;
}