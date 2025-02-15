#include <iostream>
#include <fstream>
#include "utils.h"

std::ifstream openFile(std::string filename)
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


std::string pointToString(Point& p)
{
    std::ostringstream ss;
    ss << "(" << p.x << ", " << p.y << ")";
    return ss.str();
}


Point subtractPoints(Point& a, Point& b)
{
    return Point{a.x - b.x, a.y - b.y};
}
Point addPoints(Point& a, Point& b)
{
    return Point{a.x + b.x, a.y + b.y};
}
bool isPointInBounds(Point& p, int64_t max_row, int64_t max_col)
{
    return (p.x >= 0 && p.x < max_row && p.y >= 0 && p.y < max_col);
}