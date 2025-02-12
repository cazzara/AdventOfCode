#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "utils.h"

void buildDiskMap(std::string filename, std::vector<std::string>& diskMap, std::unordered_map<std::string, int>& fileIdToSize)
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

    }
    int blockId = 0;
    int i = 0;
    int j = i + 1;
    char numBlocks, freeSpace;
    while (j < line.size())
    {
        numBlocks = line[i];
        freeSpace = line[j];
        for (int k = 0; k < charToInt(numBlocks); k++)
        {
            diskMap.push_back(std::to_string(blockId));
        }
        fileIdToSize[std::to_string(blockId)] = charToInt(numBlocks);
        for (int k = 0; k < charToInt(freeSpace); k++)
        {
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
        for (int k = 0; k < charToInt(numBlocks); k++)
        {
            diskMap.push_back(std::to_string(blockId));
        }
        fileIdToSize[std::to_string(blockId)] = charToInt(numBlocks);
    }
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

bool checkIfAllElementsFree(std::vector<std::string>& subVec)
{
    for (const auto& slot: subVec)
    {
        if (slot != ".")
        {
            return false;
        }
    }
    return true;
}

void compactifyDiskMap(std::vector<std::string>& diskMap, std::unordered_map<std::string, int>& fileIdToSize)
{
    // leftPtr holds position of left most free space
    // rightPtr holds position of right most block id
    int leftPtr = 0;
    int rtPtr = diskMap.size() - 1;
    std::unordered_set<std::string> doneFiles;
    while (leftPtr < rtPtr)
    {
        // printDiskMap(diskMap);
        while (diskMap[leftPtr] != ".")
            leftPtr++;
        while (diskMap[rtPtr] == ".")
            rtPtr--;
        // std::cout << "leftPtr="<<leftPtr<<" diskMap[leftPtr]="<<diskMap[leftPtr]<< " rtPtr="<<rtPtr<<" diskMap[rtPtr]="<<diskMap[rtPtr]<<std::endl;
        if (doneFiles.count(diskMap[rtPtr]) == 1)
        {
            // std::cout << diskMap[rtPtr] << " is done, skipping\n";
            rtPtr--;
            continue;
        }
        // get window size
        int fileSize = fileIdToSize[diskMap[rtPtr]];
        // slide window along vector
        // std::cout << "fileId=" << diskMap[rtPtr] << " fileSize=" << fileSize << std::endl;
        int idx = leftPtr; 
        bool fileFits = true;
        while (idx + fileSize < rtPtr)
        {
            while (diskMap[idx] != ".")
                idx++;
            std::vector<std::string> subVec(diskMap.begin()+idx, diskMap.begin() + (idx + fileSize));
            // std::cout << "checking subvector from " << idx << " to " << (idx + fileSize - 1) << std::endl;
            fileFits = checkIfAllElementsFree(subVec);
            if (fileFits) {
                break;
            }
            idx += 1;
        }
        doneFiles.insert(diskMap[rtPtr]);
        if (fileFits && idx + fileSize < rtPtr)
        {
            // std::cout << "Found contiguous free space for file id " << diskMap[rtPtr] << " at " << idx << std::endl;
            for (int i = 0; i < fileSize; i++)
            {
                diskMap[idx+i] = diskMap[rtPtr];
                diskMap[rtPtr] = ".";
                rtPtr--;
            }
        }
        else
        {   
            rtPtr--;
        }
    }
}

uint64_t calculateChecksum(std::vector<std::string>& diskMap)
{
    uint64_t checkSum = 0;
    for (int i = 0; i < diskMap.size(); i++)
    {
        if (diskMap[i] != ".")
            checkSum += (i * std::stoi(diskMap[i]));
    }
    return checkSum;
}

int main() 
{
    std::string filename = "../input.txt";
    std::vector<std::string> diskMap;
    std::unordered_map<std::string, int> fileIdToSize; // keep track of how many blocks a file takes up
    buildDiskMap(filename, diskMap, fileIdToSize);
    compactifyDiskMap(diskMap, fileIdToSize);
    uint64_t checkSum = calculateChecksum(diskMap);
    printDiskMap(diskMap);
    std::cout << "checksum="<<checkSum<<std::endl;
    return 0;
}