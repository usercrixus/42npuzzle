#include "Heuristic.hpp"
#include "geometry/point.hpp"

#include <cmath>
#include <iostream>

Heuristic::Heuristic(int s) : size(s), goal(s)
{
	int col = 0, row = 0;
	int dir_row = 0, dir_col = 1;
	int end = size - 1, start = 0;
	goal_points.push_back(Point{-1, -1});
	for (int i = 1; i < size * size; i++)
	{
		goal.setGrid(row, col, i);
		goal_points.push_back(Point{row, col});
		nextSnail(row, col, dir_row, dir_col, start, end);
	}
	goal.setGrid(row, col, 0);
}

Heuristic::~Heuristic() {}

bool Heuristic::isSolvable(const NPuzzle &puzzle) const
{
	std::vector<int> flat;
	std::vector<int> flat_goal;
	int				 inv = 0;
	int				 zeroRow = -1;

	std::cout << size << std::endl;
	for (int row = 0; row < size; ++row)
	{
		for (int col = 0; col < size; ++col)
		{
			int v = puzzle.getPuzzle()[row][col];
			if (v == 0)
				zeroRow = row + 1;
			else
			{
				flat.push_back(v);
				flat_goal.push_back(goal.getPuzzle()[row][col]);
			}
		}
	}

	for (size_t i = 0; i < flat.size(); ++i)
		for (size_t j = i + 1; j < flat.size(); ++j)
			if (flat[i] > flat_goal[j])
				++inv;

	if (size % 2 == 1)
		return (inv % 2) == 0;
	else
		return (inv + (size - zeroRow)) % 2 == 0;
}

bool Heuristic::isGoal(const NPuzzle &puzzle) const
{
	std::vector grid = puzzle.getPuzzle();
	std::vector goal_grid = goal.getPuzzle();

	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			if (grid[row][col] != goal_grid[row][col])
				return false;
	return true;
}

void Heuristic::nextSnail(int &row, int &col, int &dir_row, int &dir_col, int &start, int &end) const
{
	if (col == end and dir_col == 1)
	{
		dir_row = 1;
		dir_col = 0;
	}
	else if (row == end and dir_row == 1)
	{
		dir_row = 0;
		dir_col = -1;
		end -= 1;
	}
	else if (col == start and dir_col == -1)
	{
		dir_row = -1;
		dir_col = 0;
		start += 1;
	}
	else if (row == start and dir_row == -1)
	{
		dir_row = 0;
		dir_col = 1;
	}
	row += dir_row;
	col += dir_col;
}

ManhattanDistance::ManhattanDistance(int s) : Heuristic(s) {}

int ManhattanDistance::calc(const NPuzzle &puzzle)
{
	int			sum = 0;
	std::vector grid = puzzle.getPuzzle();
	for (int row = 0; row < size; ++row)
		for (int col = 0; col < size; ++col)
		{
			int value = grid[row][col];
			if (value > 0)
			{
				int targetRow = goal_points[value].x;
				int targetCol = goal_points[value].y;
				sum += abs(row - targetRow) + abs(col - targetCol);
			}
		}
	return sum;
}

MisplacedTitles::MisplacedTitles(int s) : Heuristic(s) {}

int MisplacedTitles::calc(const NPuzzle &puzzle)
{
	int			col = 0, row = 0;
	int			dir_row = 0, dir_col = 1;
	int			end = size - 1, start = 0;
	int			count = 0;
	std::vector grid = puzzle.getPuzzle();
	for (int i = 1; i < size * size; i++)
	{
		if (grid[row][col] != i)
			++count;
		nextSnail(row, col, dir_row, dir_col, start, end);
	}
	return count;
}

LinearConflicts::LinearConflicts(int s) : ManhattanDistance(s) {}

int LinearConflicts::calc(const NPuzzle &puzzle)
{
	return ManhattanDistance::calc(puzzle) + linearConflictRow(puzzle) + linearConflictColumn(puzzle);
}

int LinearConflicts::linearConflictRow(const NPuzzle &puzzle) const
{
	int			conflict = 0;
	std::vector grid = puzzle.getPuzzle();
	for (int row = 0; row < size; ++row)
	{
		for (int col = 0; col < size; ++col)
		{
			int tile1 = grid[row][col];
			if (tile1 == 0)
				continue;
			Point goal1 = goal_points[tile1];
			if (goal1.y != row)
				continue;
			for (int colOnRight = col + 1; colOnRight < size; ++colOnRight)
			{
				int tile2 = grid[row][colOnRight];
				if (tile2 == 0)
					continue;
				Point goal2 = goal_points[tile2];
				if (goal2.y != row)
					continue;
				if (goal1.x > goal2.x)
					conflict += 2;
			}
		}
	}
	return conflict;
}

int LinearConflicts::linearConflictColumn(const NPuzzle &puzzle) const
{
	int			conflict = 0;
	std::vector grid = puzzle.getPuzzle();
	for (int col = 0; col < size; ++col)
	{
		for (int row = 0; row < size; ++row)
		{
			int tile1 = grid[row][col];
			if (tile1 == 0)
				continue;
			Point goal1 = goal_points[tile1];
			if (goal1.x != col)
				continue;

			for (int rowDown = row + 1; rowDown < size; ++rowDown)
			{
				int tile2 = grid[rowDown][col];
				if (tile2 == 0)
					continue;
				Point goal2 = goal_points[tile2];
				if (goal2.x != col)
					continue;

				if (goal1.y > goal2.y)
					conflict += 2;
			}
		}
	}
	return conflict;
}
