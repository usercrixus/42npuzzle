// solveWithAStar.hpp
#pragma once

#include "NPuzzle.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>

class AStarSolver
{
private:
	struct ANode;
	struct PQItem;
	std::vector<ANode> allNodes;
	std::priority_queue<PQItem> open;
	std::unordered_map<std::string, int> stepSoFar;
	int goalIdx = -1;
public:
	AStarSolver(/* args */);
	~AStarSolver();

    void initializeSolver(const NPuzzle &puzzle);
    bool solveWithAStar(const NPuzzle& start);
	std::vector<NPuzzle::Move> getActionsPath();
};

struct AStarSolver::PQItem
{
	int id;
	int totalEstimation;
	bool operator<(PQItem const &o) const
	{
		return totalEstimation > o.totalEstimation;
	}
};

struct AStarSolver::ANode
{
	NPuzzle board;
	int g;         // cost so far
	int f;         // g + h
	int parentIdx; // index in allNodes
	NPuzzle::Move moveFromParent;
};