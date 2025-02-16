#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "utils.h"

Point UP = {-1, 0};
Point DOWN = {1, 0};
Point LEFT = {0, -1};
Point RIGHT = {0, 1};
std::unordered_map<Point, std::string, PointHash, PointEqual> pointStrMap = {
    {UP, "UP"},
    {DOWN, "DOWN"},
    {LEFT, "LEFT"},
    {RIGHT, "RIGHT"}
};
const int TRAIL_BEGIN = 0;
const int TRAIL_END = 9;


void buildTopoMap(std::string& filename, std::vector<std::vector<int>>& topoMap, std::vector<Point>& trailHeads, std::vector<Point>& peaks)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int row = 0;
    while(std::getline(inp, line))
    {  
        std::vector<int> mapRow;
        int col = 0;
        for (const auto& x: line)
        {
            int pos = std::stoi(std::string(1, x));
            if (pos == TRAIL_BEGIN)
            {
                Point p = {row, col};
                std::cout << "Trail Head found at " << pointToString(p) << std::endl;
                trailHeads.push_back(p);
            }
            if (pos == TRAIL_END)
            {
                Point p = {row, col};
                std::cout << "Peak found at " << pointToString(p) << std::endl;
                peaks.push_back(p);
            }
            mapRow.push_back(pos);
            col++;
        }
        topoMap.push_back(mapRow);
        row++;
    }
    inp.close();
}

/**
 * A move is valid if the next position is within the bound of the map and 
 * the value at the next position is strictly 1 greater than the value at the current position
 */
bool isMoveValid(Point& pos, Point& movement, std::vector<std::vector<int>>& topoMap)
{
    Point nextPos = addPoints(pos, movement);
    if (!isPointInBounds(nextPos, topoMap.size(), topoMap[0].size()))
        return false;
    int currentVal = topoMap[pos.x][pos.y];
    int nextVal = topoMap[nextPos.x][nextPos.y];
    if (nextVal - currentVal != 1)
        return false;
    return true;
}

int navigateTrail(Point& start, Point& direction, std::vector<std::vector<int>>& topoMap, std::unordered_set<Point, PointHash, PointEqual>& visited)
{
    Point newPos = addPoints(start, direction);
    if (visited.count(newPos) == 1)
    {
        std::cout << "Already visited " << pointToString(newPos) << "\n";
        return 0;
    }
    std::cout << "Moving from " << pointToString(start) << " " << pointStrMap[direction]  << " to " << pointToString(newPos) <<  std::endl;
    if (!isMoveValid(start, direction, topoMap))
    {
        std::cout << pointToString(newPos) << " invalid\n";
        return 0;
    }
    int val = topoMap[newPos.x][newPos.y];
    if (val == TRAIL_END)
    {
        std::cout << "Reached peak at " << pointToString(newPos)  <<  std::endl;
        // visited.insert(newPos);
        return 1;
    }
    return navigateTrail(newPos, UP, topoMap, visited) + navigateTrail(newPos, DOWN, topoMap, visited) + navigateTrail(newPos, LEFT, topoMap, visited) + navigateTrail(newPos, RIGHT, topoMap, visited);
}

int calculateTrailHeadScore(Point& start, std::vector<std::vector<int>>& topoMap)
{
    std::unordered_set<Point, PointHash, PointEqual> visited;
    return navigateTrail(start, UP, topoMap, visited) + navigateTrail(start, DOWN, topoMap, visited) + navigateTrail(start, LEFT, topoMap, visited) + navigateTrail(start, RIGHT, topoMap, visited);
}


int main() 
{
    std::vector<std::vector<int>> topoMap;
    std::vector<Point> trailHeads;
    std::vector<int> scores;
    std::vector<Point> peaks;
    std::string filename = "../input.txt";
    buildTopoMap(filename, topoMap, trailHeads, peaks);
    for (Point& trailHead : trailHeads)
    {
        std::cout << "Navigating starting at trailhead:" << pointToString(trailHead) << std::endl;
        int trailHeadScore = calculateTrailHeadScore(trailHead, topoMap);
        std::cout << "Found trailhead score: " << trailHeadScore << "\n";
        scores.push_back(trailHeadScore);
    }
    int sum = 0;
    for (const int& i : scores)
        sum += i;
    std::cout << "Sum of scores: " << sum << std::endl;
    return 0;
}