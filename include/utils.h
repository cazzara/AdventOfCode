#ifndef _UTILS_H
#define _UTILS_H

#include <fstream>
#include <sstream>

std::ifstream openFile(std::string filename);

int charToInt(char c);

struct Point {
    int64_t x;
    int64_t y;
};

struct PointHash {
    size_t operator()(const Point& p) const {
        size_t h1 = std::hash<int64_t>()(p.x);
        size_t h2 = std::hash<int64_t>()(p.y);
        return h1 ^ (h2 << 1);
    }
};

struct PointEqual {
    bool operator()(const Point& p1, const Point& p2) const {
        return p1.x == p2.x && p1.y == p2.y;
    }
};

extern Point UP;
extern Point DOWN;
extern Point LEFT;
extern Point RIGHT;

extern const std::unordered_map<Point, std::string, PointHash, PointEqual> pointStrMap;

std::string pointToString(Point& p);
Point subtractPoints(Point& a, Point& b);
Point addPoints(Point& a, Point& b);
bool isPointInBounds(Point& p, int64_t max_row, int64_t max_col);

#endif