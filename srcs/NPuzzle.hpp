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
    int heuristicMode;

    using HeuristicFunction = int (NPuzzle::*)(const NPuzzle&) const;
    int estimateManhattan(const NPuzzle &other) const;
    int estimateMisplacedTiles(const NPuzzle &other) const;
    int estimateLinearConflict(const NPuzzle &other) const;
public:
    NPuzzle(int heuristicMode);
    ~NPuzzle();

    enum Move { LEFT, RIGHT, TOP, BOTTOM };
    bool parse(std::string path);
    bool parse();
    void print() const;
    bool isSolvable() const;
    bool isGoal() const;
    Point<int> getZero() const;
    std::vector<Move> getMove(Point<int> zero) const;
    std::string flatten() const;
    NPuzzle applyMove(Move move) const;
    HeuristicFunction getHeuristicFunction() const;

    const std::vector<std::vector<int>>& getPuzzle() const;
};