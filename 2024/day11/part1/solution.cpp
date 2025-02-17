#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

#include "utils.h"

int TIMES_TO_BLINK = 25;

void buildInput(std::string& filename, std::vector<std::string>& stones)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    std::string stone;
    while (std::getline(inp, line))
    {
        std::stringstream ss(line);
        while(ss >> stone)
        {
            stones.push_back(stone);
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
std::vector<std::string> blink(std::vector<std::string>& stones)
{
    std::vector<std::string> newStones;
    for (const auto& stone : stones)
    {
        if (stone == "0")
            newStones.push_back("1");
        else if (stone.size() % 2 == 0)
        {
            std::string left = std::string(stone.begin(), stone.begin() + (stone.size()/2));
            std::string right = std::string(stone.begin() + (stone.size()/2), stone.end());
            newStones.push_back(left);
            newStones.push_back(std::to_string(std::stoll(right)));
        }
        else
        {
            int64_t conv = std::stoll(stone) * 2024;
            newStones.push_back(std::to_string(conv));
        }
    }
    return newStones;
}

void printStones(std::vector<std::string>& stones)
{
    std::cout << "stones: ";
    for(const auto& s: stones)
        std::cout << s << " ";
    std::cout << std::endl;
}

int main() 
{
    std::string filename = "../input.txt";
    std::vector<std::string> stones;
    buildInput(filename, stones);
    // printStones(stones);
    for (int i = 0; i < TIMES_TO_BLINK; i++)
    {
        stones = blink(stones);
        // printStones(stones);
        std::cout << "Blinked " << i << " times" << std::endl; 
    }
    std::cout << "There are " << stones.size() << " stones after blinking " << TIMES_TO_BLINK << " times.\n";
    return 0;
}