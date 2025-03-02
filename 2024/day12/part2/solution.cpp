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
 * Get all contiguous points for given row or col
 * If row < 0 ignore 
 * If col < 0 ignore
 */
std::unordered_set<Point, PointHash, PointEqual> getPoints(Point& p, std::vector<std::string>& garden, int64_t row, int64_t col)
{
    std::unordered_set<Point, PointHash, PointEqual> matches;
    char plant = garden[p.x][p.y];
    matches.insert(p);
    // Get all contiguous points for row/col
    if (row >= 0)
    {
        // std::cout << "Getting plants in row at point " << pointToString(p) << std::endl;
        int mvCol = p.y+1;
        Point newPoint = {p.x, mvCol};
        while(isMoveValid(plant, newPoint, garden))
        {
            matches.insert(newPoint);
            mvCol += 1;
            newPoint = {p.x, mvCol};
        }
        mvCol = p.y-1;
        newPoint = {p.x, mvCol};
        while(isMoveValid(plant, newPoint, garden))
        {
            matches.insert(newPoint);
            mvCol -= 1;
            newPoint = {p.x, mvCol};
        }
    }
    else if (col >= 0)
    {
        // std::cout << "Getting plants in col at point " << pointToString(p) << std::endl;
        int mvRow = p.x+1;
        Point newPoint = {mvRow, p.y};
        while(isMoveValid(plant, newPoint, garden))
        {
            matches.insert(newPoint);
            mvRow += 1;
            newPoint = {mvRow, p.y};
        }
        mvRow = p.x-1;
        newPoint = {mvRow, p.y};
        while(isMoveValid(plant, newPoint, garden))
        {
            matches.insert(newPoint);
            mvRow -= 1;
            newPoint = {mvRow, p.y};
        }
    }

    return matches;
}

/**
 * An edge point is outside of the bounds of the garden or adjacent to a different plant from the region
 */
bool isEdgePoint(Point& p, std::vector<std::string>& garden, char currentPlant)
{
    if (!isPointInBounds(p, garden.size(), garden[0].size()))
        return true;
    char adjacentPlant = garden[p.x][p.y];
    if (currentPlant != adjacentPlant)
        return true;
    return false;
}

int getRowPerimeter(std::unordered_set<Point, PointHash, PointEqual>& row, std::vector<std::string>& garden, char currentPlant)
{
    int perimeter = 0;
    std::unordered_set<Point, PointHash, PointEqual> visited;
    bool edgeFound = false;
    std::cout << "++++++Getting perimeter for row points++++++\n";
    for (Point p: row)
    {
        std::cout << pointToString(p) << std::endl;
    }
    for (Point p : row)
    {
        if (visited.count(p) == 1)
            continue;
        std::cout << "Checking point " << pointToString(p) << std::endl;
        Point o = {p.x, p.y};
        // Check the UP edge
        std::cout << "UP edge check, moving LEFT\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, UP);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of an UP edge\n";
            visited.insert(o);
            o = addPoints(o, LEFT);
            edgeFound = true;
        }
        o = addPoints(p, RIGHT);
        std::cout << "UP edge check, moving RIGHT\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, UP);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of an UP edge\n";
            visited.insert(o);
            o = addPoints(o, RIGHT);
            edgeFound = true;
        }
        if (edgeFound)
        {
            edgeFound = false;
            perimeter += 1;
        }
        visited.insert(p);
    }
    visited.clear();
    for (Point p : row)
    {
        if (visited.count(p) == 1)
            continue;
        std::cout << "Checking point " << pointToString(p) << std::endl;
        // Check DOWN edge
        Point o = {p.x, p.y};
        std::cout << "DOWN edge check, moving LEFT\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, DOWN);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a DOWN edge\n";
            visited.insert(o);
            o = addPoints(o, LEFT);
            edgeFound = true;
        }
        o = addPoints(p, RIGHT);
        std::cout << "DOWN edge check, moving RIGHT\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, DOWN);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a DOWN edge\n";
            visited.insert(o);
            o = addPoints(o, RIGHT);
            edgeFound = true;
        }
        if (edgeFound)
        {
            edgeFound = false;
            perimeter += 1;
        }
        visited.insert(p);
    }
    return perimeter;
}

int getColPerimeter(std::unordered_set<Point, PointHash, PointEqual>& col, std::vector<std::string>& garden, char currentPlant)
{
    int perimeter = 0;
    std::unordered_set<Point, PointHash, PointEqual> visited;
    bool edgeFound = false;
    std::cout << "++++++Getting perimeter for col points++++++\n";
    for (Point p: col)
    {
        std::cout << pointToString(p) << std::endl;
    }
    for (Point p : col)
    {
        if (visited.count(p) == 1)
            continue;
        std::cout << "Checking point " << pointToString(p) << std::endl;
        Point o = {p.x, p.y};
        // Check the right edge
        std::cout << "RIGHT edge check, moving UP\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, RIGHT);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a RIGHT edge\n";
            visited.insert(o);
            o = addPoints(o, UP);
            edgeFound = true;
        }
        o = addPoints(p, DOWN);
        std::cout << "RIGHT edge check, moving DOWN\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, RIGHT);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a RIGHT edge\n";
            visited.insert(o);
            o = addPoints(o, DOWN);
            edgeFound = true;
        }
        if (edgeFound)
        {
            edgeFound = false;
            perimeter += 1;
        }
    }
    visited.clear();
    for (Point p : col)
    {
        if (visited.count(p) == 1)
            continue;
        // Check the left edge
        Point o = {p.x, p.y};
        std::cout << "LEFT edge check, moving UP\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, LEFT);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a LEFT edge\n";
            visited.insert(o);
            o = addPoints(o, UP);
            edgeFound = true;
        }
        o = addPoints(p, DOWN);
        std::cout << "LEFT edge check, moving DOWN\n";
        while(isMoveValid(currentPlant, o, garden) && visited.count(o) != 1)
        {
            Point r = addPoints(o, LEFT);
            if (!isEdgePoint(r, garden, currentPlant))
                break;
            std::cout << "Point " << pointToString(o) << " is part of a LEFT edge\n";
            visited.insert(o);
            o = addPoints(o, DOWN);
            edgeFound = true;
        }
        if (edgeFound)
        {
            edgeFound = false;
            perimeter += 1;
        }
        visited.insert(p);
    }
    return perimeter;
}


/**
 * Perimeter value is incremented for each side that borders a plant that is not within the current region or the edge of the garden
 */
int perimeterValue(Region& r, std::vector<std::string>& garden)
{
    std::cout << "Getting perimeter for region: " << r.plantType << std::endl;
    // for (Point p : r.subplots)
    // {
    //     std::cout << pointToString(p) << std::endl;
    // }
    int perimeter = 0;
    std::unordered_set<Point, PointHash, PointEqual> visited;
    // Row
    for (Point p : r.subplots)
    {
        // If we've already calculated the perimeter for the points in this row, skip it
        if (visited.count(p) == 1)
            continue;
        // Get all points for this row
        std::unordered_set<Point, PointHash, PointEqual> pointsInRow = getPoints(p, garden, p.x, -1);
        // std::cout << "Row " << p.x << " contains the following points:\n";
        for (Point pr: pointsInRow)
        {
            // std::cout << pointToString(pr) << std::endl;
            visited.insert(pr);
        }
        perimeter += getRowPerimeter(pointsInRow, garden, r.plantType);
        std::cout << "Perimeter is now " << perimeter << std::endl;

    }
    visited.clear();
    // Cols
    for (Point p : r.subplots)
    {
        if (visited.count(p) == 1)
            continue;
        std::unordered_set<Point, PointHash, PointEqual> pointsInCol = getPoints(p, garden, -1, p.y);
        // std::cout << "Col " << p.y << " contains the following points:\n";
        for (Point c: pointsInCol)
        {
            // std::cout << pointToString(c) << std::endl;
            visited.insert(c);
        }
        perimeter += getColPerimeter(pointsInCol, garden, r.plantType);
        std::cout << "Perimeter is now " << perimeter << std::endl;

    }
    return perimeter;
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
    r.area++;
    r.subplots.insert(o);
    traverseGarden(o, UP, r, garden);
    traverseGarden(o, DOWN, r, garden);
    traverseGarden(o, LEFT, r, garden);
    traverseGarden(o, RIGHT, r, garden); 
}

Region carveRegion(Point& p, std::vector<std::string>& garden)
{
    
    char regionPlant = garden[p.x][p.y];
    Region r = {regionPlant, 1, 0, std::unordered_set<Point, PointHash, PointEqual>{p}};
    traverseGarden(p, UP, r, garden);
    traverseGarden(p, DOWN, r, garden);
    traverseGarden(p, LEFT, r, garden);
    traverseGarden(p, RIGHT, r, garden);
    int perimeter = perimeterValue(r, garden);
    std::cout << "Region " << r.plantType << " has perimeter " << perimeter << "\n";
    r.perimeter = perimeter;
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