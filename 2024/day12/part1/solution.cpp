#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "utils.h"

struct Region {
    char plantType;                // type of plant
    int64_t area;                  // number of plots in the region
    int64_t perimeter;             // For each plot, count of number of sides that are not touching the region
    std::unordered_set<Point, PointHash, PointEqual> subplots;   // all plots (points) in the region
};

void buildGarden(std::string& filename, std::vector<std::string>& garden, std::unordered_set<Point, PointHash, PointEqual>& plots)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    int row = 0;
    while(std::getline(inp, line))
    {
        garden.push_back(line);
        for (int col = 0; col < static_cast<int>(line.size()); col++)
        {
            plots.insert({row, col});
        }
        row++;
    }
}

/**
 * A valid move is in bounds of the garden and within the same plant as the current region
 */
bool isMoveValid(char targetPlant, Point& p, std::vector<std::string>& garden)
{
    if (!isPointInBounds(p, garden.size(), garden[0].size()))
        return false;
    char currentPlant = garden[p.x][p.y];
    if (currentPlant != targetPlant)
        return false;
    return true;
}

/**
 * Perimeter value is incremented for each side that borders a plant that is not within the current region or the edge of the garden
 */
int perimeterValue(Point& p, std::vector<std::string>& garden)
{
    int value = 0;
    char currentPlant = garden[p.x][p.y];
    Point o = addPoints(p, UP);
    if (!isPointInBounds(o, garden.size(), garden[0].size()) || garden[o.x][o.y] != currentPlant)
        value++;
    o = addPoints(p, DOWN);
    if (!isPointInBounds(o, garden.size(), garden[0].size()) || garden[o.x][o.y] != currentPlant)
        value++;
    o = addPoints(p, LEFT);
    if (!isPointInBounds(o, garden.size(), garden[0].size()) || garden[o.x][o.y] != currentPlant)
        value++;
    o = addPoints(p, RIGHT);
    if (!isPointInBounds(o, garden.size(), garden[0].size()) || garden[o.x][o.y] != currentPlant)
        value++;
    return value;
}



void traverseGarden(Point& p, Point& direction, Region& r, std::vector<std::string>& garden)
{
    Point o = addPoints(p, direction);
    if (r.subplots.count(o) == 1)
    {
        // std::cout << "Already checked point " << pointToString(o) << std::endl; 
        return;
    }
    // std::cout << "Checking point " << pointToString(o) << std::endl; 
    if (!isMoveValid(r.plantType, o, garden))
    {
        // std::cout << "Point " << pointToString(o) << " is invalid." << std::endl; 
        return;
    }
    int perimeter = perimeterValue(o, garden);
    // std::cout << "Point " << pointToString(o) << " has perimeter value: " << perimeter << std::endl;
    r.perimeter += perimeter;
    r.area++;
    r.subplots.insert(o);
    traverseGarden(o, UP, r, garden);
    traverseGarden(o, DOWN, r, garden);
    traverseGarden(o, LEFT, r, garden);
    traverseGarden(o, RIGHT, r, garden); 
}

Region carveRegion(Point& p, std::vector<std::string>& garden)
{
    int perimeter = perimeterValue(p, garden);
    char regionPlant = garden[p.x][p.y];
    Region r = {regionPlant, 1, perimeter, std::unordered_set<Point, PointHash, PointEqual>{p}};
    traverseGarden(p, UP, r, garden);
    traverseGarden(p, DOWN, r, garden);
    traverseGarden(p, LEFT, r, garden);
    traverseGarden(p, RIGHT, r, garden);
    return r;
}

int64_t calculateFencePrices(std::vector<std::string>& garden, std::unordered_set<Point, PointHash, PointEqual>& plots)
{
    int64_t fencePrice = 0;
    while(!plots.empty())
    {
        Point start = *plots.begin();
        Region r = carveRegion(start, garden);
        std::cout << "Region of plant " << r.plantType << " area: " << r.area << " perimeter: " << r.perimeter << " price: " << r.area * r.perimeter << std::endl;
        fencePrice += r.area * r.perimeter;
        for (const Point p : r.subplots)
        {
            plots.erase(p);
        }
    }
    return fencePrice;
}

int main()
{
    std::string filename = "../input.txt";
    std::vector<std::string> garden;
    std::unordered_set<Point, PointHash, PointEqual> plots;
    buildGarden(filename, garden, plots);
    int64_t price = calculateFencePrices(garden, plots);
    std::cout << "Total price is : " << price << std::endl;
    return 0;
}