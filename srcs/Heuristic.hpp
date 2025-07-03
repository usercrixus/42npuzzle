#pragma once

#include <vector>

#include "NPuzzle.hpp"
#include "geometry/point.hpp"

class NPuzzle;

class Heuristic
{
  public:
	Heuristic(int size);
	virtual ~Heuristic();
	virtual int calc(const NPuzzle &puzzle) = 0;
	bool		isGoal(const NPuzzle &puzzle) const;

  protected:
	int				   size;
	NPuzzle			   goal;
	std::vector<Point> goal_points;
	void			   nextSnail(int &row, int &col, int &dir_row, int &dir_col, int &start, int &end) const;
};

class ManhattanDistance : public Heuristic
{
  public:
	ManhattanDistance(int size);
	int calc(const NPuzzle &puzzle);
};

class MisplacedTitles : public Heuristic
{
  public:
	MisplacedTitles(int size);
	int calc(const NPuzzle &puzzle);
};

class LinearConflicts : public ManhattanDistance
{
  public:
	LinearConflicts(int size);

	int calc(const NPuzzle &puzzle);
	int linearConflictRow(const NPuzzle &puzzle) const;
	int linearConflictColumn(const NPuzzle &puzzle) const;
};
