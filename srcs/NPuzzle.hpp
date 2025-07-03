#pragma once

#include "geometry/point.hpp"
#include <string>
#include <vector>

class NPuzzle
{
  private:
	std::vector<std::vector<int> > puzzle;
	int							   size;
	std::string					   strGrid;
	std::string					   flatten() const;
	Point						   zero;

  public:
	NPuzzle(int size);
	NPuzzle(const std::string &path);
	~NPuzzle();

	using Grid = const std::vector<std::vector<int> >;

	enum Move
	{
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		NONE,
	};

	void	print() const;
	bool	isSolvable() const;
	NPuzzle applyMove(Move move) const;

	NPuzzle::Grid	  &getPuzzle() const;
	const std::string &getFlatten() const;

	void			  setGrid(int row, int col, int value);
	Point			  getZero() const;
	std::vector<Move> getMove() const;
	int				  getSize() const;
};
