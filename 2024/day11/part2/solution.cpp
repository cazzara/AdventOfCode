#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <sstream>

#include "utils.h"

int TIMES_TO_BLINK = 75;


void buildInput(std::string& filename, std::map<std::string, int64_t>& stones)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    std::string stone;
    while (std::getline(inp, line))
    {
        std::stringstream ss(line);
        while(ss >> stone)
        {
            stones[stone] = 1;
        }
    }
}

/**
 * If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.
 * If the stone is engraved with a number that has an even number of digits, it is replaced by two stones. 
 * The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone. 
 * (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)
 * If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.
 */
std::map<std::string, int64_t> blink(std::map<std::string, int64_t>& stones)
{
    std::map<std::string, int64_t> newStones;
    std::vector<std::string> removeStones;
    for (auto& pair : stones)
    {
        std::string s = pair.first;
        if (s == "0")
        {
            newStones["1"] += pair.second;
        }
        else if (s.size() % 2 == 0)
        {
            std::string left = std::string(s.begin(), s.begin() + (s.size()/2));
            std::string right = std::string(s.begin() + (s.size()/2), s.end());
            std::string sl = left;
            std::string sr = std::to_string(std::stoll(right));
            newStones[sl] += pair.second;
            newStones[sr] += pair.second;
        }
        else
        {
            std::string s2 = std::to_string(std::stoll(s) * 2024);
            newStones[s2] += pair.second;
        }
    }
    return newStones;
}

int main() 
{
    std::string filename = "../input.txt";
    std::map<std::string, int64_t> stones;
    buildInput(filename, stones);
    for (int i = 0; i < TIMES_TO_BLINK; i++)
    {
        stones = blink(stones);
        std::cout << "+++  Blinked " << i << " times +++" << std::endl; 
    }
    int64_t numStones = 0;
    for (const auto& pair : stones)
    {   
        numStones += pair.second;
    }
    std::cout << "There are " << numStones << " stones after blinking " << TIMES_TO_BLINK << " times.\n";
    return 0;
}