#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>

#include "utils.h"

const char UP = '^';
const char DOWN = 'v';
const char RIGHT = '>';
const char LEFT = '<';
const char OBSTACLE = '#';
const char NOT_VISITED = '.';
const char VISITED = 'X';

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
                guardPosition.push_back(row);
                guardPosition.push_back(col);
            }
        }
        row++;
    }
}

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

int traverseMap(std::vector<std::string>& labMap, std::vector<int>& startPosition)
{
    int row = startPosition[0];
    int col = startPosition[1];
    char currentDirection = labMap[row][col];
    std::map<std::tuple<int,int>, int> positionCounts;
    while(true)
    {
        if (positionCounts[std::make_tuple(row, col)] >= 1000)
            return true;
        std::tie(row, col) = move(row, col, currentDirection);
        if (!inBounds(row, col, labMap))
            break;
        if (labMap[row][col] == OBSTACLE)
        {
            std::tie(row, col) = move(row, col, oppositeDirectionMap.at(currentDirection));
            currentDirection = newDirectionMap.at(currentDirection);
            continue;
        }
        positionCounts[std::make_tuple(row, col)]++;
    }
}

void getPossibleObstaclePositions(std::vector<std::string>& labMap, std::vector<int>& startPosition, std::set<std::tuple<int, int>>& obstaclePositions)
{
    int row = startPosition[0];
    int col = startPosition[1];
    char currentDirection = labMap[row][col];
    while (true)
    {
        std::tie(row, col) = move(row, col, currentDirection);
        if (!inBounds(row, col, labMap))
            break;
        if (labMap[row][col] == OBSTACLE)
        {
            std::tie(row, col) = move(row, col, oppositeDirectionMap.at(currentDirection));
            currentDirection = newDirectionMap.at(currentDirection);
            continue;
        }
        if (labMap[row][col] == NOT_VISITED)
        {
            if (row != startPosition[0] && col != startPosition[1])
                obstaclePositions.insert(std::make_tuple(row, col));
        }
    }
}

int main()
{
    std::string filename = "../input.txt";
    std::vector<int> guardPosition;
    std::vector<std::string> labMap;
    std::set<std::tuple<int, int>> obstaclePositions;
    int numLoops = 0;
    buildPuzzleInput(filename, labMap, guardPosition);
    getPossibleObstaclePositions(labMap, guardPosition, obstaclePositions);
    int row, col;
    int checked = 0;
    for (const auto& obstaclePosition : obstaclePositions)
    {
        std::tie(row, col) = obstaclePosition;
        labMap[row][col] = OBSTACLE;
        bool loopDetected = traverseMap(labMap, guardPosition);
        if (loopDetected)
        {
            std::cout << "Loop Detected! " << row << " " << col << std::endl;
            numLoops++;
        }
        labMap[row][col] = NOT_VISITED;
        checked++;
    }

    std::cout << "Number of loops: " << numLoops << std::endl;
    return 0;
}