#include "AStarSolver.hpp"
#include "ANode.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

AStarSolver::AStarSolver(const options &opts, NPuzzle start) : opts(opts)
{
	if (opts.heuristicMode == 0)
		heuristic = new ManhattanDistance(start.getSize());
	else if (opts.heuristicMode == 1)
		heuristic = new MissplacedTitles(start.getSize());
	else if (opts.heuristicMode == 2)
		heuristic = new LinearConflicts(start.getSize());
	else
		throw std::runtime_error(
			"invalid heuristicMode {0:Manhattan distance, 1: Missplaced titles, 3: Linear conflicts");
	auto node = new ANode(start, heuristic->calc(start));
	openQueue.push(node);
	openMap[node->hash()] = node;
}

AStarSolver::~AStarSolver()
{
	delete heuristic;
	for (auto i : openMap)
		delete i.second;
	for (auto i : closedSet)
		delete i.second;
}

bool AStarSolver::solve()
{
	while (!openQueue.empty())
	{
		ANode *current = openQueue.top();
		openQueue.pop();
		openMap.erase(current->hash());
		if (closedSet.find(current->hash()) == closedSet.end())
		{
			closedSet[current->hash()] = current;
			if (heuristic->calc(current->getPuzzle()) == 0)
			{
				endNode = current;
				return true;
			}
			pushSolverNodes(current);
		}
	}
	return false;
}

void AStarSolver::pushSolverNodes(ANode *current)
{
	NPuzzle board = current->getPuzzle();

	for (auto mv : current->getPuzzle().getMove())
	{
		NPuzzle		next = board.applyMove(mv);
		std::string k = next.getFlatten();
		if (closedSet.find(k) != closedSet.end())
			continue;
		numberOfStateSelected++;
		maxNumberOfStateInMemory = std::max(maxNumberOfStateInMemory, openQueue.size());
		ANode *node;
		if (openMap.find(k) == openMap.end())
		{
			node = new ANode(next, mv, current);
			if (!opts.greedy)
				node->setG(current->getG() + 1);
			if (!opts.uniform)
				node->setH(heuristic->calc(next));
			openQueue.push(node);
			openMap[k] = node;
		}
		else if (openMap[k]->getG() > current->getG() + 1)
		{
			node = openMap[k];
			if (!opts.greedy)
				node->setG(current->getG() + 1);
			if (!opts.uniform)
				node->setH(heuristic->calc(next));
			node->setParent(current, mv);
		}
	}
}

std::vector<NPuzzle::Move> AStarSolver::getActionsPath() const
{
	std::vector<NPuzzle::Move> path;
	ANode					  *curr = endNode;

	while (curr && curr->getParent())
	{
		path.push_back(curr->getMove());
		curr = curr->getParent();
	}
	std::reverse(path.begin(), path.end());
	return path;
}

int AStarSolver::getNumberOfStateSelected() const
{
	return numberOfStateSelected;
}

int AStarSolver::getMaxnumberOfStateInMemory() const
{
	return maxNumberOfStateInMemory;
}

void AStarSolver::printInfo() const
{
	auto solution = getActionsPath();

	std::cout << "===== Summary =====" << std::endl;
	std::cout << "Solution moves (" << solution.size() << "):" << std::endl;
	std::cout << "Number of state selected (" << getNumberOfStateSelected() << "):" << std::endl;
	std::cout << "Max number of state in memory (" << getMaxnumberOfStateInMemory() << "):" << std::endl;
	std::cout << "Solution:" << std::endl;
}

void AStarSolver::printSolution() const
{
	const char *const moveToString[] = {"LEFT", "RIGHT", "TOP", "BOTTOM"};
	auto			  solution = getActionsPath();

	if (solution.empty())
		std::cout << "No solution found (this should never happen for a solvable puzzle)." << std::endl;
	else
	{
		for (auto move : solution)
			std::cout << moveToString[move] << " ";
		std::cout << std::endl;
	}
}
