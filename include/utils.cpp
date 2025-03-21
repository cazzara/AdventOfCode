#include <iostream>
#include <fstream>
#include "utils.h"

std::ifstream openFile(const std::string& filename)
{
    std::ifstream inp{filename};
    if (!inp.is_open())
    {
        throw std::runtime_error("unable to open file: " + filename);
    }
    return inp;
}

int charToInt(char c)
{
    return c - '0';
}


std::string pointToString(const Point& p)
{
    std::ostringstream ss;
    ss << "(" << p.x << ", " << p.y << ")";
    return ss.str();
}


Point subtractPoints(const Point& a, const Point& b)
{
    return Point{a.x - b.x, a.y - b.y};
}
Point addPoints(const Point& a, const Point& b)
{
    return Point{a.x + b.x, a.y + b.y};
}
bool isPointInBounds(const Point& p, const int64_t max_row, const int64_t max_col)
{
    return (p.x >= 0 && p.x < max_row && p.y >= 0 && p.y < max_col);
}

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