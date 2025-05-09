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
	std::priority_queue<PQItem> openNodes;
	std::unordered_map<std::string, int> stepSoFarMap;
	int goalIdx = -1;
public:
	AStarSolver(/* args */);
	~AStarSolver();

    void initializeSolver(const NPuzzle &puzzle);
    bool isSolved(int bestNodeId);
    void pushSolverNodes(int bestNodeId);
    bool solveWithAStar(const NPuzzle &start);
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
	int parentIdx; // index in allNodes
	NPuzzle::Move moveFromParent;
};