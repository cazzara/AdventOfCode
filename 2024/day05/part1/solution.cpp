#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <regex>

#include "utils.h"

void extractRule(std::unordered_map<int, std::unordered_set<int>>& rules, std::string rule)
{
    std::regex ruleRegex{"(\\d+)\\|(\\d+)"};
    std::smatch sm;
    std::regex_search(rule, sm, ruleRegex);
    int x = std::stoi(sm[1].str());
    int y = std::stoi(sm[2].str());
    std::unordered_set<int> vals = rules[x];
    vals.insert(y);
    rules[x] = vals;
}

std::vector<int> extractPageUpdates(std::string update)
{
    std::regex updateRegex{"(\\d+)"};
    std::vector<int> updates;
    for (std::smatch sm; std::regex_search(update, sm, updateRegex);)
    {
        updates.push_back(std::stoi(sm.str()));
        update = sm.suffix();
    }
    return updates;
}

void buildPuzzleInput(std::string filename, std::unordered_map<int, std::unordered_set<int>>& rules, std::vector<std::vector<int>>& pageNumbers)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    bool rulesInput = true;
    while (std::getline(inp, line))
    {
        if (line == "")
        {
            rulesInput = false;
            continue;
        }
        if (rulesInput)
        {
            extractRule(rules, line);
        }
        else
        {
            std::vector<int> page = extractPageUpdates(line);
            pageNumbers.push_back(page);
        }
    }
}

bool isPageOrdered(std::vector<int> page, std::unordered_map<int, std::unordered_set<int>>& rules)
{
    int idx = 1;
    while (idx < page.size())
    {
        int currentPage = page[idx];
        int prev = 0;
        while (prev < idx)
        {
            int prevPage = page[prev];
            std::unordered_set<int> pageRules = rules[prevPage];
            if (pageRules.count(currentPage) == 0)
                return false;
            prev++;
        }
        idx++;
    }
    return true;
}

int main()
{
    std::string filename = "../test_input.txt";
    std::unordered_map<int, std::unordered_set<int>> rules;
    std::vector<std::vector<int>> pageNumbers;
    buildPuzzleInput(filename, rules, pageNumbers);
    int orderedPages = 0;
    int middleSum = 0;
    for (std::vector<int> page: pageNumbers)
    {
        if (isPageOrdered(page, rules))
        {
            orderedPages++;
            int mid = page.size() / 2;
            int midPage = page[mid];
            middleSum += midPage;
        }
    }
    std::cout << "ordered pages: " << orderedPages << " middle sum: " << middleSum << std::endl;
    return 0;
}