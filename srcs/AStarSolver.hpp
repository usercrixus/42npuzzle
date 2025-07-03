#pragma once

#include <map>
#include <queue>
#include <unordered_map>
#include <vector>

#include "ANode.hpp"
#include "Heuristic.hpp"
#include "NPuzzle.hpp"
#include "parser/arguments.hpp"

class AStarSolver
{
  private:
	std::priority_queue<ANodeQueue>			 openQueue;
	std::map<std::string, ANode *>			 openMap;
	std::unordered_map<std::string, ANode *> closedSet;
	size_t									 numberOfStateSelected = 0;
	size_t									 maxNumberOfStateInMemory = 0;
	Heuristic								*heuristic;
	options									 opts;
	ANode									*endNode;

	void pushSolverNodes(ANode *current);

  public:
	AStarSolver(const options &ops, NPuzzle start);
	~AStarSolver();

	bool					   solve();
	int						   getNumberOfStateSelected() const;
	int						   getMaxnumberOfStateInMemory() const;
	std::vector<NPuzzle::Move> getActionsPath() const;
	void					   printSolution() const;
	void					   printInfo() const;
};
