#pragma once

#include "NPuzzle.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <unordered_set>

class AStarSolver
{
private:
	struct ANode;
	struct PQItem;
	std::vector<ANode> allNodes;
	std::priority_queue<PQItem> openNodes;
	std::unordered_set<std::string> closedSet;
	int goalIdx = -1;
	std::size_t numberOfStateSelected = 0;
	std::size_t maxNumberOfStateInMemory = 0;
public:
	AStarSolver(/* args */);
	~AStarSolver();

    void initializeSolver(const NPuzzle &puzzle);
    bool isSolved(int bestNodeId);
    void pushSolverNodes(const AStarSolver::PQItem &node);
    bool solveWithAStar(const NPuzzle &start);
    std::vector<NPuzzle::Move> getActionsPath();
	int getNumberOfStateSelected();
	int getMaxnumberOfStateInMemory();
};

struct AStarSolver::PQItem
{
	int id;
	int stepSoFar;
	int totalEstimation;
	bool operator<(PQItem const &o) const
	{
		return totalEstimation > o.totalEstimation;
	}
};

struct AStarSolver::ANode
{
	NPuzzle board;
	int parentIdx;
	NPuzzle::Move moveFromParent;
};