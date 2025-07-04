#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>

#include "Heuristic.hpp"
#include "NPuzzle.hpp"
#include "geometry/point.hpp"
#include "parser/FileToVector.hpp"

NPuzzle::NPuzzle(int size) : size(size)
{
	std::vector<int> buffer(size * size);
	std::iota(buffer.begin(), buffer.end(), 0);
	std::random_device rd;
	std::mt19937	   rng(rd());
	std::shuffle(buffer.begin(), buffer.end(), rng);
	puzzle.assign(size, std::vector<int>(size));
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			puzzle[i][j] = buffer[i * size + j];
	strGrid = flatten();
	zero = getZero();
}

NPuzzle::NPuzzle(const std::string &path)
{
	FileToVector<int> parser;
	parser.parse(path);
	auto const &tokens = parser.getTokens();
	if (tokens.size() < 2)
		throw std::runtime_error("incorrect file format");
	size = tokens[0];
	if (tokens.size() - 1 != (size_t)size * size)
		throw std::runtime_error("incorrect file format");
	puzzle.assign(size, std::vector<int>());
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			puzzle[i].push_back(tokens[1 + i * size + j]);
	strGrid = flatten();
	zero = getZero();
}

std::string NPuzzle::flatten() const
{
	std::ostringstream oss;
	for (const auto &row : puzzle)
		for (int v : row)
			oss << v << ",";
	return oss.str();
}

const std::string &NPuzzle::getFlatten() const
{
	return strGrid;
}

NPuzzle::~NPuzzle() {}

void NPuzzle::print() const
{
	for (auto const &row : puzzle)
	{
		for (int v : row)
			std::cout << v << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// bool NPuzzle::isSolvable() const
// {
// 	std::vector<int> flat;
// 	int				 inv = 0;
// 	int				 zeroRow = -1;
//
// 	for (int row = 0; row < size; ++row)
// 	{
// 		for (int col = 0; col < size; ++col)
// 		{
// 			int v = puzzle[row][col];
// 			if (v == 0)
// 				zeroRow = row;
// 			else
// 				flat.push_back(v);
// 		}
// 	}
//
// 	for (size_t i = 0; i < flat.size(); ++i)
// 		for (size_t j = i + 1; j < flat.size(); ++j)
// 			if (flat[i] > flat[j])
// 				++inv;
//
// 	if (size % 2 == 1)
// 		return (inv % 2) == 0;
// 	else
// 		return (inv + (size - zeroRow)) % 2 == 0;
// }

Point NPuzzle::getZero() const
{
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			if (puzzle[i][j] == 0)
				return Point{j, i};
	return Point{-1, -1};
}

std::vector<NPuzzle::Move> NPuzzle::getMove() const
{
	std::vector<Move> mv;
	int				  x = zero.x, y = zero.y;
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
	int		x = zero.x, y = zero.y;

	if (m == LEFT)
	{
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x - 1]);
		nxt.zero.x = x - 1;
	}
	else if (m == RIGHT)
	{
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x + 1]);
		nxt.zero.x = x + 1;
	}
	else if (m == TOP)
	{
		std::swap(nxt.puzzle[y][x], nxt.puzzle[y - 1][x]);
		nxt.zero.y = y - 1;
	}
	else if (m == BOTTOM)
	{

		std::swap(nxt.puzzle[y][x], nxt.puzzle[y + 1][x]);
		nxt.zero.y = y + 1;
	}
	nxt.strGrid = nxt.flatten();
	return nxt;
}

void NPuzzle::setGrid(int row, int col, int value)
{
	puzzle[row][col] = value;
}

const std::vector<std::vector<int> > &NPuzzle::getPuzzle() const
{
	return puzzle;
}

int NPuzzle::getSize() const
{
	return size;
}
