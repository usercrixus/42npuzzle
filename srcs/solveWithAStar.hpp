// solveWithAStar.hpp
#pragma once

#include "NPuzzle.hpp"
#include <vector>

/// Finds the shortest path from `start` to the goal using A* (Manhattan).
/// Returns empty if unsolvable or already solved.
std::vector<NPuzzle::Move> solveWithAStar(const NPuzzle& start);
