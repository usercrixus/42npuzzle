// solveWithAStar.cpp
#include "AStarSolver.hpp"

AStarSolver::AStarSolver()
{
}

AStarSolver::~AStarSolver()
{
}

void AStarSolver::initializeSolver(const NPuzzle &puzzle)
{
	auto hFunc = puzzle.getHeuristicFunction();
	int initialEstimation = (puzzle.*hFunc)(puzzle);
	allNodes.push_back({puzzle, -1, NPuzzle::LEFT});
	openNodes.push({0, 0, initialEstimation});
}

bool AStarSolver::isSolved(int bestNodeId)
{
	if (allNodes[bestNodeId].board.isGoal())
	{
		goalIdx = bestNodeId;
		return (true);
	}
	return (false);
}

void AStarSolver::pushSolverNodes(const AStarSolver::PQItem &node)
{
	const NPuzzle board = allNodes[node.id].board;
	int stepSoFar = node.stepSoFar;
	for (auto mv : board.getMove(board.getZero()))
	{
		NPuzzle next = board.applyMove(mv);
		std::string key = next.flatten();
		if (!closedSet.count(key))
		{
			numberOfStateSelected++;
			maxNumberOfStateInMemory = std::max(maxNumberOfStateInMemory, openNodes.size());
			auto hFunc = next.getHeuristicFunction();
			int totalEstimation;
			if (numberOfStateSelected > 999999)
				totalEstimation = (next.*hFunc)(next);
			else
				totalEstimation = stepSoFar + 1 + (next.*hFunc)(next);
			int nextIdx = allNodes.size();
			allNodes.push_back({next, node.id, mv});
			openNodes.push({nextIdx, stepSoFar + 1, totalEstimation});
		}
	}
}


bool AStarSolver::solveWithAStar(const NPuzzle &puzzle)
{
	initializeSolver(puzzle);
	while (!openNodes.empty())
	{
		const AStarSolver::PQItem bestNode = openNodes.top();
		openNodes.pop();
		const std::string &key = allNodes[bestNode.id].board.flatten();
		if (!closedSet.count(key))
		{
			closedSet.insert(key);
			if (isSolved(bestNode.id))
				return true;
			pushSolverNodes(bestNode);
		}
	}
	return false;
}


std::vector<NPuzzle::Move> AStarSolver::getActionsPath()
{
	std::vector<NPuzzle::Move> path;
	if (goalIdx != -1)
	{
		for (int at = goalIdx; at > 0; at = allNodes[at].parentIdx)
		{
			path.push_back(allNodes[at].moveFromParent);
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

int AStarSolver::getNumberOfStateSelected()
{
    return (numberOfStateSelected);
}

int AStarSolver::getMaxnumberOfStateInMemory()
{
    return (maxNumberOfStateInMemory);
}
