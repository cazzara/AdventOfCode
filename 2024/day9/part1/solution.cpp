#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int charToInt(char c)
{
    return c - '0';
}

char intToChar(int i)
{

    return i + '0';
}

void buildDiskMap(std::string filename, std::vector<std::string>& diskMap)
{
    std::ifstream inp{filename};
    if (!inp.is_open())
    {
        std::cout << "unable to open file " << filename << std::endl;
        return;
    }
    std::string line;
    while(std::getline(inp, line))
    {  
        // std::cout << line << std::endl;
    }
    int blockId = 0;
    int i = 0;
    int j = i + 1;
    char numBlocks, freeSpace;
    while (j < line.size())
    {
        numBlocks = line[i];
        freeSpace = line[j];
        // std::cout << "numBlocks: " << numBlocks << " freeSpace: " << freeSpace << "\n";
        for (int k = 0; k < charToInt(numBlocks); k++)
        {
            // std::cout << "adding " << blockId << std::endl;
            diskMap.push_back(std::to_string(blockId));
        }
        for (int k = 0; k < charToInt(freeSpace); k++)
        {
            // std::cout << "adding ." << std::endl;
            diskMap.push_back(".");
        }
        blockId++;
        i+=2;
        j+=2;
    }
    // if input is odd length, there will be one last item at pos i
    if (line.size() % 2 != 0)
    {
        numBlocks = line[i];
        // std::cout << "numBlocks: " << numBlocks << std::endl;
        for (int k = 0; k < charToInt(numBlocks); k++)
        {
            // std::cout << "adding " << blockId << std::endl;
            diskMap.push_back(std::to_string(blockId));
        }
    }
    // std::cout << "Printing\n";
    // for (int x = diskMap.size(); x > diskMap.size() - 50; x--)
    // {
    //     std::cout << x << " " << diskMap[x] << std::endl;
    // } 
    inp.close();
}

void printDiskMap(std::vector<std::string>& diskMap)
{
    for (const auto& x: diskMap)
    {
        std::cout << x;
    }
    std::cout << std::endl;
}

void compactifyDiskMap(std::vector<std::string>& diskMap)
{
    // leftPtr holds position of left most free space
    // rightPtr holds position of right most block id
    int leftPtr = 0;
    int rtPtr = diskMap.size() - 1;
    // move blocks to the left most free space
    // printDiskMap(diskMap);
    while (leftPtr < rtPtr)
    {
        while (diskMap[leftPtr] != ".")
            leftPtr++;
        while (diskMap[rtPtr] == ".")
            rtPtr--;
        if (leftPtr >= rtPtr)
            break;
        diskMap[leftPtr] = diskMap[rtPtr];
        diskMap[rtPtr] = ".";
        leftPtr++;
        rtPtr--;
    }
}

uint64_t calculateChecksum(std::vector<std::string>& diskMap)
{
    uint64_t checkSum = 0;
    for (int i = 0; i < diskMap.size(); i++)
    {
        if (diskMap[i] == ".")
            break;
        checkSum += (i * std::stoi(diskMap[i]));
    }
    return checkSum;
}

int main() 
{
    std::string filename = "input.txt";
    std::vector<std::string> diskMap;
    buildDiskMap(filename, diskMap);
    compactifyDiskMap(diskMap);
    uint64_t checkSum = calculateChecksum(diskMap);
    std::cout << "checksum="<<checkSum<<std::endl;
    return 0;
}