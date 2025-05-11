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

	void initializeSolver(const NPuzzle &puzzle);
    bool isSolved(int bestNodeId);
    void pushSolverNodes(int bestNodeId);

public:
	AStarSolver(/* args */);
	~AStarSolver();

    bool solveWithAStar(const NPuzzle &start);
	int getNumberOfStateSelected();
	int getMaxnumberOfStateInMemory();
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
	int stepSoFar;
	int parentIdx;
	NPuzzle::Move moveFromParent;
};