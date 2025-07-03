#include "ANode.hpp"
#include "NPuzzle.hpp"

ANode::ANode(NPuzzle puzzle, int h) : puzzle(puzzle), h(h) {}

ANode::ANode(NPuzzle puzzle, NPuzzle::Move move, ANode *parent) : puzzle(puzzle), parent(parent), moveFromParent(move)
{
}

int ANode::f() const
{
	return g + h;
}

const std::string &ANode::hash() const
{
	return puzzle.getFlatten();
}

const NPuzzle &ANode::getPuzzle() const
{
	return puzzle;
}

NPuzzle::Move ANode::getMove() const
{
	return moveFromParent;
}

int ANode::getG() const
{
	return g;
}

ANode *ANode::getParent() const
{
	return parent;
}

void ANode::setH(int h)
{
	this->h = h;
}

void ANode::setG(int g)
{
	this->g = g;
}

void ANode::setParent(ANode *parent, NPuzzle::Move mv)
{
	this->parent = parent;
	this->moveFromParent = mv;
}

bool ANodeCompare::operator()(const ANode *lhs, const ANode *rhs) const
{
	return lhs->f() > rhs->f();
}
