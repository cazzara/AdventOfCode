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

std::vector<Point> getAntinodes(Point& a, Point& b)
{
    Point c = subtractPoints(a, b);
    Point an1 = addPoints(a, c);
    Point an2 = subtractPoints(b, c);
    return std::vector<Point>{an1, an2};
}



int main()
{
    std::string filename = "../input.txt";
    std::unordered_map<char, std::vector<Point>> antennaLocations;
    std::unordered_set<Point, PointHash, PointEqual> antinodes;
    buildAntennaLocations(filename, antennaLocations);
    std::cout << "MAX_ROW " << MAX_ROW << " MAX_COL " << MAX_COL << std::endl;
    for (const auto& pair : antennaLocations)
    {
        std::vector<Point> locations = pair.second;
        for (size_t i = 0; i < locations.size(); i++)
        {
            for (size_t j = i + 1; j < locations.size(); j++)
            {
                std::vector<Point> potentialAntinodes = getAntinodes(locations[i], locations[j]);
                for (auto& node : potentialAntinodes)
                {
                    if (isPointInBounds(node, MAX_ROW, MAX_COL))
                    {
                        antinodes.insert(node);
                    }
                }
            }
        }
    }
    std::cout << "Found " << antinodes.size() << " antinodes\n";
    return 0;
}