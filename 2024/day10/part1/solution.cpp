#include <iostream>
#include <fstream>
#include <string>
#include <vector>


void buildTopoMap(std::string filename, std::vector<std::vector<int>>& topoMap)
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
        std::vector<int> row;
        for (const auto& x: line)
        {
            row.push_back(std::stoi(std::string(1, x)));
        }
        topoMap.push_back(row);
    }
    inp.close();
}

int main() 
{
    std::string filename = "../test_input.txt";
    return 0;
}