#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils.h"


static int64_t MAX_ROW;
static int64_t MAX_COL;

void buildAntennaLocations(std::string& filename, std::unordered_map<char, std::vector<Point>>& antennaLocations)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int row = 0;
    while (std::getline(inp, line))
    {
        for (size_t col = 0; col < line.size(); col++)
        {
            if (line[col] != '.')
            {
                Point p = {row, static_cast<int64_t>(col)};
                antennaLocations[line[col]].push_back(p);
                MAX_COL = line.size();
            }
        }
        row++;
    }
    MAX_ROW = row;
}

std::vector<Point> getAntinodes(const Point& a, const Point& b)
{
    std::vector<Point> points{a, b};
    Point c = subtractPoints(a, b);
    Point an1 = addPoints(a, c);
    while (isPointInBounds(an1, MAX_ROW, MAX_COL))
    {
        points.push_back(an1);
        an1 = addPoints(an1, c);
    }
    Point an2 = subtractPoints(b, c);
    while (isPointInBounds(an2, MAX_ROW, MAX_COL))
    {
        points.push_back(an2);
        an2 = subtractPoints(an2, c);
    }
    return points;
}



int main()
{
    std::string filename = "../input.txt";
    std::unordered_map<char, std::vector<Point>> antennaLocations;
    std::unordered_set<Point, PointHash, PointEqual> antinodes;
    buildAntennaLocations(filename, antennaLocations);
    for (const auto& pair : antennaLocations)
    {
        std::vector<Point> locations = pair.second;
        for (size_t i = 0; i < locations.size(); i++)
        {
            for (size_t j = i + 1; j < locations.size(); j++)
            {
                std::vector<Point> potentialAntinodes = getAntinodes(locations[i], locations[j]);
                for (const auto& node : potentialAntinodes)
                {
                    antinodes.insert(node);
                }
            }
        }
    }
    std::cout << "Found " << antinodes.size() << " antinodes\n";
    return 0;
}