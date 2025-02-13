#include <iostream>
#include <fstream>
#include <vector>

#include "utils.h"

char UP = '^';
char DOWN = 'v';
char RIGHT = '>';
char LEFT = '<';
char OBSTACLE = '#';
char NOT_VISITED = '.';
char VISITED = 'X';

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
            if (line[col] == UP || line[col] || DOWN || line[col] == LEFT || line[col] == RIGHT)
            {
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
    bool finished = false;
    int row = guardPosition[0];
    int col = guardPosition[1];
    char currentDirection = labMap[row][col];
    while (!finished)
    {
        if (currentDirection == UP)
        {
            int newRow = row - 1;
            if (newRow < 0)
            {
                numMoves++;
                finished = true;
            }
            else if (labMap[newRow][col] == OBSTACLE)
                currentDirection = RIGHT;
            else
            {
                if (labMap[newRow][col] == NOT_VISITED)
                {
                    labMap[newRow][col] = VISITED;
                    numMoves++;
                }
                row = newRow;
            }
        }
        if (currentDirection == DOWN)
        {
            int newRow = row + 1;
            if (newRow > labMap.size())
            {
                numMoves++;
                finished = true;
            }
            else if (labMap[newRow][col] == OBSTACLE)
                currentDirection = LEFT;
            else
            {
                if (labMap[newRow][col] == NOT_VISITED)
                {
                    labMap[newRow][col] = VISITED;
                    numMoves++;
                }
                row = newRow;
            }
        }
        if (currentDirection == RIGHT)
        {
            int newCol = col + 1;
            if (newCol > labMap[0].size())
            {
                numMoves++;
                finished = true;
            }
            else if (labMap[row][newCol] == OBSTACLE)
                currentDirection = DOWN;
            else
            {
                if (labMap[row][newCol] == NOT_VISITED)
                {
                    labMap[row][newCol] = VISITED;
                    numMoves++;
                }
                col = newCol;
            }
        }
        if (currentDirection == LEFT)
        {
            int newCol = col - 1;
            if (newCol < 0)
            {
                numMoves++;
                finished = true;
            }
            else if (labMap[row][newCol] == OBSTACLE)
                currentDirection = UP;
            else
            {
                if (labMap[row][newCol] == NOT_VISITED)
                {
                    labMap[row][newCol] = VISITED;
                    numMoves++;
                }
                col = newCol;
            }
        }
    }
    return numMoves;
}

int main()
{
    std::string filename = "../test_input.txt";
    std::vector<int> guardPosition;
    std::vector<std::string> labMap;
    buildPuzzleInput(filename, labMap, guardPosition);
    int moves = traverseMap(labMap, guardPosition);
    std::cout << "Number of moves: " << moves << std::endl;
    return 0;
}