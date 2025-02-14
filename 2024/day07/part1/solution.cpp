#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <regex>

#include "utils.h";


void parseEquationsInput(std::string& filename, std::unordered_map<long, std::vector<long>>& equations)
{
    std::regex regex{("\\d+")};
    std::ifstream inp = openFile(filename);
    std::string line;
    while (std::getline(inp, line))
    {
        int i = 0;
        std::vector<long> numList;
        long targetNum;
        for (std::smatch sm; std::regex_search(line, sm, regex);)
        {
            long num = std::stol(sm.str());
            if (i == 0)
                targetNum = num;
            else
                numList.push_back(num);
            line = sm.suffix();
            i++;
        }
        equations[targetNum] = numList;
    }
}

bool work(long x, long targetNum, std::vector<long> rem)
{
    if (rem.size() == 1)
        return x + rem[0] == targetNum || x * rem[0] == targetNum;
    else
    {
        std::vector<long> newVec(rem.begin() + 1, rem.end());
        return work(x * rem[0], targetNum, newVec) || work(x + rem[0], targetNum, newVec);
    }
}

int main()
{
    std::unordered_map<long, std::vector<long>> equations;
    std::string filename = "../test_input.txt";
    parseEquationsInput(filename, equations);
    std::vector<char> operations{'+', '*'};
    long sum = 0;
    for (const auto& pair : equations)
    {
        std::vector<long> newVec(pair.second.begin()+1, pair.second.end());
        bool isVaild = work(pair.second[0], pair.first, newVec) || work(pair.second[0], pair.first, newVec);
        if (isVaild)
            sum += pair.first;
    }
    std::cout << sum << std::endl;
    return 0;
}