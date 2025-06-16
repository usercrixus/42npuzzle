#pragma once

#include <string>
#include <vector>
#include <climits>
#include <map>
#include "geometry/point.hpp"

class NPuzzle
{
private:
    std::vector<std::vector<int>> puzzle;
    int size;
    int heuristicMode;

    using HeuristicFunction = int (NPuzzle::*)() const;
    int estimateManhattan() const;
    int estimateMisplacedTiles() const;
    int estimateLinearConflict() const;
    int linearConflictRow() const;
    int linearConflictColumn() const;

    std::map<int, Point<int> >	   goalMap;
	std::map<int, Point<int> > buildGoalMap() const;
	void nextSnail(int &row, int &col, int &dir_row, int &dir_col, int &start, int &end) const;
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
