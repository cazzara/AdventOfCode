#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>

#include "utils.h"

char UP = '^';
char DOWN = 'v';
char RIGHT = '>';
char LEFT = '<';
char OBSTACLE = '#';
char NOT_VISITED = '.';
char VISITED = 'X';

const std::unordered_map<char, char> newDirectionMap = {
    {UP, RIGHT},
    {RIGHT, DOWN},
    {DOWN, LEFT},
    {LEFT, UP}
};

const std::unordered_map<char, char> oppositeDirectionMap = {
    {UP, DOWN},
    {DOWN, UP},
    {RIGHT, LEFT},
    {LEFT, RIGHT}
};

std::tuple<int, int> move(int row, int col, char direction)
{
    if (direction == UP)
        row -= 1;
    else if (direction == DOWN)
        row += 1;
    else if (direction == LEFT)
        col -= 1;
    else if (direction == RIGHT)
        col += 1;
    return std::make_tuple(row, col);
}

bool inBounds(int row, int col, std::vector<std::string>& labMap)
{
    if (row > labMap.size() || row < 0 || col > labMap[0].size() || col < 0)
        return false;
    return true;
}

void buildPuzzleInput(std::string filename, std::vector<std::string>& labMap, std::vector<int>& guardPosition)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int row = 0;
    while (std::getline(inp, line))
    {
        labMap.push_back(line);
        for (int col = 0; col < line.size(); col++)
        {
            if (line[col] == UP || line[col] == DOWN || line[col] == LEFT || line[col] == RIGHT)
            {
                std::cout << "start pos is at: " << row << " " << col << std::endl;
                guardPosition.push_back(row);
                guardPosition.push_back(col);
            }
        }
        row++;
    }
}

int traverseMap(std::vector<std::string>& labMap, std::vector<int>& guardPosition)
{
    int numMoves = 0;
    int row = guardPosition[0];
    int col = guardPosition[1];
    char currentDirection = labMap[row][col];
    while (true)
    {
        std::tie(row, col) = move(row, col, currentDirection);
        if (!inBounds(row, col, labMap))
        {
            numMoves++;
            break;
        }
        if (labMap[row][col] == OBSTACLE)
        {
            std::tie(row, col) = move(row, col, oppositeDirectionMap.at(currentDirection));
            currentDirection = newDirectionMap.at(currentDirection);
            continue;
        }
        if (labMap[row][col] == NOT_VISITED)
        {
            labMap[row][col] = VISITED;
            numMoves++;
        }
    }
    return numMoves;
}

int main()
{
    std::string filename = "../input.txt";
    std::vector<int> guardPosition;
    std::vector<std::string> labMap;
    buildPuzzleInput(filename, labMap, guardPosition);
    int moves = traverseMap(labMap, guardPosition);
    std::cout << "Number of moves: " << moves << std::endl;
    return 0;
}