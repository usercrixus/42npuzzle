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
	openNodes.push({0, initialEstimation});
	stepSoFarMap[puzzle.flatten()] = 0;
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

void AStarSolver::pushSolverNodes(int bestNodeId)
{
	const NPuzzle board = allNodes[bestNodeId].board;
	int stepSoFar = stepSoFarMap.at(board.flatten());
	std::vector<NPuzzle::Move> moves = board.getMove(board.getZero());
	for (auto mv : moves)
	{
		NPuzzle next = board.applyMove(mv);
		std::string key = next.flatten();
		if (stepSoFarMap.find(key) == stepSoFarMap.end())
		{
			numberOfStateSelected++;
			maxNumberOfStateInMemory = maxNumberOfStateInMemory > openNodes.size() ? maxNumberOfStateInMemory : openNodes.size();
			auto hFunc = next.getHeuristicFunction();
			int totalEstimation = stepSoFar + 1 + (next.*hFunc)(next);
			int nextIdx = allNodes.size();
			allNodes.push_back({next, bestNodeId, mv});
			stepSoFarMap[key] = stepSoFar + 1;;
			openNodes.push({nextIdx, totalEstimation});
		}
	}
}

bool AStarSolver::solveWithAStar(const NPuzzle &puzzle)
{
	initializeSolver(puzzle);
	while (!openNodes.empty())
	{
		int bestNodeId = openNodes.top().id;
		openNodes.pop();
		if (isSolved(bestNodeId))
			return (true);
		else
			pushSolverNodes(bestNodeId);
	}
	return (false);
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
