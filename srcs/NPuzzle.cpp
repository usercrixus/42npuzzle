#include <iostream>
#include <random>

#include "NPuzzle.hpp"
#include "parser/FileToVector.hpp"

NPuzzle::NPuzzle(int heuristicMode) : size(0), heuristicMode(heuristicMode) {}

NPuzzle::~NPuzzle() {}

bool NPuzzle::parse(std::string path)
{
	FileToVector<int> parser;
	if (!parser.parse(path))
		return false;
	auto const &tokens = parser.getTokens();
	if (tokens.size() < 2)
		return std::cout << "The file " << path << " is incorrectly formatted\n", false;
	size = tokens[0];
	if (tokens.size() - 1 != (size_t)size * size)
		return std::cout << "The file " << path << " is incorrectly formatted\n", false;
	puzzle.assign(size, std::vector<int>());
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			puzzle[i].push_back(tokens[1 + i * size + j]);
	goalMap = buildGoalMap();
	return true;
}

bool NPuzzle::parse()
{
	size = 4;
	std::vector<int> buffer(size * size);
	std::iota(buffer.begin(), buffer.end(), 0);
	std::random_device rd;
	std::mt19937	   rng(rd());
	std::shuffle(buffer.begin(), buffer.end(), rng);
	puzzle.assign(size, std::vector<int>(size));
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			puzzle[i][j] = buffer[i * size + j];
	goalMap = buildGoalMap();
	return true;
}

void NPuzzle::print() const
{
	for (auto const &row : puzzle)
	{
		for (int v : row)
			std::cout << v << " ";
		std::cout << "\n";
	}
	std::cout << "\n";
}

std::string NPuzzle::flatten() const
{
	std::ostringstream oss;
	for (const auto &row : puzzle)
		for (int v : row)
			oss << v << ",";
	return oss.str();
}

bool NPuzzle::isSolvable() const
{
	std::vector<int> flat;
	int				 blankRow = -1;
	for (int row = 0; row < size; ++row)
	{
		for (int column = 0; column < size; ++column)
		{
			int v = puzzle[row][column];
			if (v == 0)
				blankRow = row;
			else
				flat.push_back(v);
		}
	}
	int inv = 0;
	for (size_t row = 0; row < flat.size(); ++row)
		for (size_t column = row + 1; column < flat.size(); ++column)
			if (flat[row] > flat[column])
				++inv;
	if (size % 2 == 1)
		return (inv % 2) == 0;
	int rowFromBottom = size - blankRow;
	return ((inv + rowFromBottom) % 2) == 1;
}

Point<int> NPuzzle::getZero() const
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			if (puzzle[i][j] == 0)
				return Point<int>(j, i);
	return Point<int>(-1, -1);
}

std::vector<NPuzzle::Move> NPuzzle::getMove(Point<int> zero) const
{
	std::vector<Move> mv;
	int				  x = zero.getX(), y = zero.getY();
	if (x > 0)
		mv.push_back(LEFT);
	if (x < size - 1)
		mv.push_back(RIGHT);
	if (y > 0)
		mv.push_back(TOP);
	if (y < size - 1)
		mv.push_back(BOTTOM);
	return mv;
}

NPuzzle NPuzzle::applyMove(Move m) const
{
	NPuzzle nxt = *this;
	auto [x, y] = std::pair<int, int>(getZero().getX(), getZero().getY());
	if (m == LEFT)
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x - 1]);
	if (m == RIGHT)
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x + 1]);
	if (m == TOP)
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y - 1][x]);
	if (m == BOTTOM)
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y + 1][x]);
	return nxt;
}

NPuzzle::HeuristicFunction NPuzzle::getHeuristicFunction() const
{
	if (heuristicMode == 1)
		return &NPuzzle::estimateMisplacedTiles;
	if (heuristicMode == 2)
		return &NPuzzle::estimateLinearConflict;
	return &NPuzzle::estimateManhattan;
}

int NPuzzle::estimateManhattan() const
{
	int sum = 0;
	for (int row = 0; row < size; ++row)
		for (int col = 0; col < size; ++col)
		{
			int value = puzzle[row][col];
			if (value > 0)
			{
				int targetRow = goalMap.at(value).getX();
				int targetCol = goalMap.at(value).getY();
				sum += abs(row - targetRow) + abs(col - targetCol);
			}
		}
	return sum;
}

int NPuzzle::estimateMisplacedTiles() const
{
	int col = 0, row = 0;
	int dir_row = 0, dir_col = 1;
	int end = size - 1, start = 0;
	int count = 0;
	for (int i = 1; i < size * size; i++)
	{
		if (puzzle[row][col] != i)
			++count;
		nextSnail(row, col, dir_row, dir_col, start, end);
	}
	return count;
}

int NPuzzle::estimateLinearConflict() const
{
	return estimateManhattan() + linearConflictRow() + linearConflictColumn();
}

int NPuzzle::linearConflictRow() const
{
	int conflict = 0;
	for (int row = 0; row < size; ++row)
	{
		for (int col = 0; col < size; ++col)
		{
			int tile1 = puzzle[row][col];
			if (tile1 == 0)
				continue;
			Point<int> goal1 = goalMap.at(tile1);
			if (goal1.getY() != row)
				continue;
			for (int colOnRight = col + 1; colOnRight < size; ++colOnRight)
			{
				int tile2 = puzzle[row][colOnRight];
				if (tile2 == 0)
					continue;
				Point<int> goal2 = goalMap.at(tile2);
				if (goal2.getY() != row)
					continue;
				if (goal1.getX() > goal2.getX())
					conflict += 2;
			}
		}
	}
	return conflict;
}

int NPuzzle::linearConflictColumn() const
{
	int conflict = 0;
	for (int col = 0; col < size; ++col)
	{
		for (int row = 0; row < size; ++row)
		{
			int tile1 = puzzle[row][col];
			if (tile1 == 0)
				continue;
			Point<int> goal1 = goalMap.at(tile1);
			if (goal1.getX() != col)
				continue;

			for (int rowDown = row + 1; rowDown < size; ++rowDown)
			{
				int tile2 = puzzle[rowDown][col];
				if (tile2 == 0)
					continue;
				Point<int> goal2 = goalMap.at(tile2);
				if (goal2.getX() != col)
					continue;

				if (goal1.getY() > goal2.getY())
					conflict += 2;
			}
		}
	}
	return conflict;
}

const std::vector<std::vector<int> > &NPuzzle::getPuzzle() const
{
	return puzzle;
}

std::map<int, Point<int> > NPuzzle::buildGoalMap() const
{
	std::map<int, Point<int> > ret;
	int						   col = 0, row = 0;
	int						   dir_row = 0, dir_col = 1;
	int						   end = size - 1, start = 0;
	for (int i = 1; i < size * size; i++)
	{
		ret[i] = Point<int>(row, col);
		nextSnail(row, col, dir_row, dir_col, start, end);
	}
	return ret;
}

void NPuzzle::nextSnail(int &row, int &col, int &dir_row, int &dir_col, int &start, int &end) const
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

bool NPuzzle::isGoal() const
{
	int col = 0, row = 0;
	int dir_row = 0, dir_col = 1;
	int end = size - 1, start = 0;
	for (int i = 1; i < size * size; i++)
	{
		if (puzzle[row][col] != i)
			return false;
		nextSnail(row, col, dir_row, dir_col, start, end);
	}
	return true;
}
