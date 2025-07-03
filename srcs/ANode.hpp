#pragma once

#include "NPuzzle.hpp"
#include <string>

#define ANodeQueue ANode *, std::vector<ANode *>, ANodeCompare

class ANode
{
  private:
	NPuzzle		  puzzle;
	int			  h = 0;
	int			  g = 0;
	ANode		 *parent = NULL;
	NPuzzle::Move moveFromParent = NPuzzle::NONE;
	bool		  valid = true;

  public:
	ANode() {};
	ANode(NPuzzle puzzle, int h);
	ANode(NPuzzle puzzle, NPuzzle::Move move, ANode *parent);
	int				   f() const;
	const std::string &hash() const;

	bool		   getValid() const;
	const NPuzzle &getPuzzle() const;
	NPuzzle::Move  getMove() const;
	int			   getG() const;
	int			   getH() const;
	ANode		  *getParent() const;

	void setInvalid();
	void setH(int h);
	void setG(int g);
	void setParent(ANode *parent, NPuzzle::Move mv);
};

struct ANodeCompare
{
	bool operator()(const ANode *lhs, const ANode *rhs) const;
};
