#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <cmath>

#include "utils.h"

struct Game {
    Point A;      // Point representing the amount of movement X/Y moved by button A
    Point B;      // Point representing the amount of movement X/Y moved by button B
    Point prize;  // Point representing the location of the prize for this game
};

int BUTTON_A_COST = 3;
int BUTTON_B_COST = 1;
int64_t PRIZE_OFFSET = 10000000000000;
int64_t MAX_BUTTON_PRESS = PRIZE_OFFSET;
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
    int64_t x = std::stoi(sm[1].str()) + PRIZE_OFFSET;
    int64_t y = std::stoi(sm[2].str()) + PRIZE_OFFSET;
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
        Point A = parseButton(game[i]);
        Point B = parseButton(game[i+1]);
        Point prize = parsePrize(game[i+2]);
        Game g = {A, B, prize};
        games.push_back(g);
        i += 3;
    }
}

// https://openstax.org/books/intermediate-algebra-2e/pages/4-6-solve-systems-of-equations-using-determinants
int64_t findLowestCostToPrize(Game& g)
{
    int64_t M[2][2] = {
        // a     b
        {g.A.x, g.B.x},  // X
        // c     d
        {g.A.y, g.B.y}   // Y
    };
    // Calculate determinant ad - bc
    int64_t D = (M[0][0] * M[1][1]) - (M[0][1] * M[1][0]);
    if (D == 0)
        return -1;

    int64_t B[2][1] = {
        {g.prize.x},
        {g.prize.y}
    };
    // First column of M is replaced by B
    long double  D_x = (B[0][0] * M[1][1]) - (B[1][0] * M[0][1]);
    // Second column of M is replaced by B
    long double  D_y = (M[0][0] * B[1][0]) - (M[1][0] * B[0][0]);
    long double a = D_x / D;
    long double b = D_y / D;
    if (std::floor(a) == a && std::floor(b) == b)
        return (a * BUTTON_A_COST) + (b * BUTTON_B_COST);
    return -1;
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
        if (cost == -1)
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
    std::cout << "Total cost: " << total_cost << std::endl;
    return 0;
}