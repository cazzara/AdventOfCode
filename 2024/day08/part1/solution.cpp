#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

struct Point{
    int x;
    int y;
};

// Hash function for Point struct
struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t h1 = std::hash<int>()(p.x);
        std::size_t h2 = std::hash<int>()(p.y);
        return h1 ^ (h2 << 1); // Simple hash combining x and y
    }
};

// Equality comparison for Point struct
struct PointEqual {
    bool operator()(const Point& p1, const Point& p2) const {
        return p1.x == p2.x && p1.y == p2.y;
    }
};



void buildAntennaLocations(std::string filename, std::unordered_map<char, std::vector<Point> >& antennaLocations, std::vector<int>& maxRowCol)
{
    std::ifstream inputFile{filename};
    std::string line;
    int row = 0;
    while(std::getline(inputFile, line))
    {
        for (int col = 0; col < line.size(); col++)
        {
            if (line[col] != '.')
            {
                std::cout << "Found antenna " << line[col] << " at " << row << " " << col << std::endl;
                Point p = {row, col};
                antennaLocations[line[col]].push_back(p);
            }
        }
        row++;
    }
    maxRowCol.push_back(row);
    maxRowCol.push_back(line.size());
}

Point subtractPoints(Point a, Point b)
{
    return Point{a.x - b.x, a.y - b.y};
}
Point addPoints(Point a, Point b)
{
    return Point{a.x + b.x, a.y + b.y};
}

bool checkPointBounds(Point p, int maxRow, int maxCol)
{
    if (p.x < 0 || p.x >= maxRow || p.y < 0 || p.y >= maxCol)
        return false;
    return true;
}

std::vector<Point> getAntinodes(Point a, Point b)
{
    
    Point c = subtractPoints(a, b);
    Point an1 = addPoints(a, c);
    Point an2 = subtractPoints(b, c);
    return std::vector<Point>{an1, an2};
}

void printPoint(Point p)
{
    std::cout << "  (" << p.x << ", " << p.y << ")"<< std::endl;
}


int main()
{
    std::unordered_map<char, std::vector<Point> > antennaLocations;
    std::vector<int> maxRowCol;
    std::unordered_set<Point, PointHash, PointEqual> antinodes;
    std::string filename = "test_input.txt";
    buildAntennaLocations(filename, antennaLocations, maxRowCol);
    std::cout << "Max Row: " << maxRowCol[0] << " Max Col: " << maxRowCol[1] << std::endl;
    for (const auto& pair : antennaLocations)
    {
        std::cout << "Positions for antenna: " << pair.first << std::endl;
        for (const auto& p : pair.second)
        {
            printPoint(p);
        }
    }

    return 0;
}