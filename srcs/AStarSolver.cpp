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
    allNodes.push_back({puzzle, 0, initialEstimation, -1, NPuzzle::LEFT});
    open.push({0, initialEstimation});
    stepSoFar[puzzle.flatten()] = 0;
}

bool AStarSolver::solveWithAStar(const NPuzzle &puzzle)
{
	initializeSolver(puzzle);
    std::string key;
    while (!open.empty())
    {
        AStarSolver::PQItem bestTmpNode = open.top();
        open.pop();
        AStarSolver::ANode &node = allNodes[bestTmpNode.id];

        if (node.board.isGoal())
            return (goalIdx = bestTmpNode.id, true);
        else
        {
            Point<int> zeroPos = node.board.getZero();
            for (NPuzzle::Move mv : node.board.getMove(zeroPos))
            {
                NPuzzle next = node.board.applyMove(mv);
                key = next.flatten();
                int currentStep = node.g + 1;
                std::unordered_map<std::string, int>::iterator it = stepSoFar.find(key);
                if (it == stepSoFar.end() || currentStep < it->second)
                {
                    int totalEstimation = currentStep + next.estimate(next);
                    int nextIdx = allNodes.size();
                    allNodes.push_back({next, currentStep, totalEstimation, bestTmpNode.id, mv});
                    stepSoFar[key] = currentStep;
                    open.push({nextIdx, totalEstimation});
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
