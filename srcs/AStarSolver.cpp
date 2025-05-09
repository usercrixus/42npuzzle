// solveWithAStar.cpp
#include "AStarSolver.hpp"

AStarSolver::AStarSolver()
{
	allNodes.reserve(20000);
}

AStarSolver::~AStarSolver()
{
}

void AStarSolver::initializeSolver(const NPuzzle &puzzle)
{
	int initialEstimation = puzzle.estimate(puzzle);
	allNodes.push_back({puzzle, -1, NPuzzle::LEFT});
	openNodes.push({0, initialEstimation});
	stepSoFarMap[puzzle.flatten()] = 0;
}

bool AStarSolver::solveWithAStar(const NPuzzle &puzzle)
{
	initializeSolver(puzzle);
	while (!openNodes.empty())
	{
		int bestNodeId = openNodes.top().id;
		openNodes.pop();
		AStarSolver::ANode &bestNode = allNodes[bestNodeId];
		if (bestNode.board.isGoal())
		{
			goalIdx = bestNodeId;
			return (true);
		}
		else
		{
			int stepSoFar = stepSoFarMap[bestNode.board.flatten()];
			for (NPuzzle::Move mv : bestNode.board.getMove(bestNode.board.getZero()))
			{
				NPuzzle next = bestNode.board.applyMove(mv);
				std::string nextKey = next.flatten();
				std::unordered_map<std::string, int>::iterator it = stepSoFarMap.find(nextKey);
				if (it == stepSoFarMap.end())
				{
					int totalEstimation = stepSoFar + 1 + next.estimate(next);
					int nextIdx = allNodes.size();
					allNodes.push_back({next, bestNodeId, mv});
					stepSoFarMap[nextKey] = stepSoFar + 1;
					openNodes.push({nextIdx, totalEstimation});
				}
			}
		}
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
