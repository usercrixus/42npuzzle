#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <climits>
#include <random>
#include <chrono>
#include "parser/FileToVector.hpp"
#include "geometry/point.hpp"

class NPuzzle
{
private:
    std::vector<std::vector<int>> puzzle;
    int size;

public:
    enum Move { LEFT, RIGHT, TOP, BOTTOM };

    NPuzzle();
    ~NPuzzle();

    bool parse(std::string path);
    bool parse();
    void print() const;
    Point<int> getZero() const;
    std::vector<Move> getMove(Point<int> zero) const;
    NPuzzle applyMove(Move move) const;

    bool isGoal() const;
    bool isSolvable() const;
    std::pair<Move, int> getBestMove() const;
    int estimate(const NPuzzle &other) const;

    const std::vector<std::vector<int>>& getPuzzle() const;
    std::string flatten() const;
};