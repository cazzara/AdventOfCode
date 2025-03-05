#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <limits>

#include "utils.h"

struct Game {
    Point A;      // Point representing the amount of movement X/Y moved by button A
    Point B;      // Point representing the amount of movement X/Y moved by button B
    Point prize;  // Point representing the location of the prize for this game
};

int BUTTON_A_COST = 3;
int BUTTON_B_COST = 1;
int MAX_MOVES = 100;
int64_t MAX_INT = std::numeric_limits<int64_t>::max();

std::string gameToString(Game& g)
{
    return "Button A: " + pointToString(g.A) + " Button B: " + pointToString(g.B) + " Prize: " + pointToString(g.prize) + "\n";
}

Point parseButton(const std::string& line)
{
    std::regex btnRegex{"X\\+(\\d+), Y\\+(\\d+)"};
    std::smatch sm;
    std::regex_search(line, sm, btnRegex);
    int x = std::stoi(sm[1].str());
    int y = std::stoi(sm[2].str());
    return {x, y};
}

Point parsePrize(const std::string& line)
{
    std::regex prizeRegex{"X=(\\d+), Y=(\\d+)"};
    std::smatch sm;
    std::regex_search(line, sm, prizeRegex);
    int x = std::stoi(sm[1].str());
    int y = std::stoi(sm[2].str());
    return {x, y};
}

void buildPuzzleInput(const std::string& filename, std::vector<Game>& games)
{
    std::ifstream inp = openFile(filename);
    std::string line;
    std::vector<std::string> game;
    while (std::getline(inp, line))
    {
        if (line != "")
            game.push_back(line);
    }
    size_t i = 0;
    while (i < game.size())
    {
        // std::cout << "parsing button A\n";
        // std::cout << game[i] << std::endl;
        Point A = parseButton(game[i]);
        // std::cout << "parsing button B\n";
        // std::cout << game[i+1] << std::endl;
        Point B = parseButton(game[i+1]);
        // std::cout << "parsing prize\n";
        // std::cout << game[i+2] << std::endl;
        Point prize = parsePrize(game[i+2]);
        Game g = {A, B, prize};
        // std::cout << gameToString(g);
        games.push_back(g);
        i += 3;
    }
}

int64_t lowestCostRecur(Game& g, Point pos, int cost, int aPresses, int bPresses, std::unordered_map<Point, int, PointHash, PointEqual>& pointCostMap)
{
    // std::cout << "Current pos: " << pointToString(pos)  << " Prize Location: " << pointToString(g.prize) << " cost: " << cost << " aPresses: " << aPresses << " bPresses: " << bPresses << "\n";
    Point presses = {aPresses, bPresses};
    if (pointCostMap.count(presses) == 1)
    {
        // std::cout << "Already pressed buttons " << pointToString(presses) << " returning cost: " << pointCostMap[presses] << std::endl;
        return MAX_INT;
    }

    if (aPresses >= MAX_MOVES || bPresses >= MAX_MOVES)
    {
        // std::cout << "Exceeded button presses. aPresses: " << aPresses << " bPresses: " << bPresses << "\n";
        pointCostMap[presses] = MAX_INT;
        return MAX_INT;
    }
    if (pos.x > g.prize.x || pos.y > g.prize.y)
    {
        // std::cout << "Overshot prize!\n";
        pointCostMap[presses] = MAX_INT;
        return MAX_INT;
    }
    // std::cout << "Setting presses " << pointToString(presses) << " cost: " << cost << std::endl;
    pointCostMap[presses] = cost;
    PointEqual isEqual;
    if (isEqual(pos, g.prize))
    {
        // std::cout << "+++++++Reached prize!+++++++ cost: " << cost << std::endl;
        return cost;
    }
    return std::min(lowestCostRecur(g, addPoints(pos, g.A), cost + BUTTON_A_COST, aPresses + 1, bPresses, pointCostMap), lowestCostRecur(g, addPoints(pos, g.B), cost + BUTTON_B_COST, aPresses, bPresses + 1, pointCostMap));
}

int64_t findLowestCostToPrize(Game& g)
{
    Point pos = {0,0};
    int64_t cost = 0;
    int aPresses = 0;
    int bPresses = 0;
    std::unordered_map<Point, int, PointHash, PointEqual> pointCostMap;
    return std::min(lowestCostRecur(g, addPoints(pos, g.A), cost + BUTTON_A_COST, aPresses + 1, bPresses, pointCostMap), lowestCostRecur(g, addPoints(pos, g.B), cost + BUTTON_B_COST, aPresses, bPresses + 1, pointCostMap));
}

int main()
{
    std::string filename = "../input.txt";
    std::vector<Game> games;
    std::vector<int64_t> costs;
    buildPuzzleInput(filename, games);
    for (Game g : games)
    {
        std::cout << "Finding cost for game " << gameToString(g);
        int64_t cost = findLowestCostToPrize(g);
        if (cost == MAX_INT)
            std::cout << "Unable to reach prize.\n";
        else
        {
            std::cout << "Cost is " << cost << "\n";
            costs.push_back(cost);
        }
    }
    int64_t total_cost = 0;
    for (int64_t cost : costs)
        total_cost += cost;
    std::cout << "total cost: " << total_cost << std::endl;
    return 0;
}