#include "NPuzzle.hpp"
#include "AStarSolver.hpp"
#include <iostream>
#include <string>

std::string moveToString(NPuzzle::Move move)
{
    switch (move)
    {
    case NPuzzle::LEFT:
        return "LEFT";
    case NPuzzle::RIGHT:
        return "RIGHT";
    case NPuzzle::TOP:
        return "TOP";
    case NPuzzle::BOTTOM:
        return "BOTTOM";
    default:
        return "UNKNOWN";
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
        return (std::cerr << "Usage: " << argv[0] << " <puzzle_file>\n", 1);
    NPuzzle puzzle;
    if (!puzzle.parse(argv[1]))
        return (std::cerr << "Failed to parse puzzle file.\n", 1);
    std::cout << "====Initial puzzle====\n";
    puzzle.print();
    if (!puzzle.isSolvable())
        return (std::cout << "This puzzle is unsolvable (odd inversion count).\n", 0);
    if (puzzle.isGoal())
        return (std::cout << "Puzzle is already solved!\n", 0);
    AStarSolver s;
    s.solveWithAStar(puzzle);
    auto solution = s.getActionsPath();
    if (solution.empty())
        std::cout << "No solution found (this should never happen for a solvable puzzle).\n";
    else
    {
        std::cout << "=====SUMMARY====\n";
        std::cout << "Solution moves (" << solution.size() << "):\n";
        std::cout << "Number of state selected (" << s.getNumberOfStateSelected() << "):\n";
        std::cout << "Max number of state in memory (" << s.getMaxnumberOfStateInMemory() << "):\n";
        std::cout << "Solution:\n";
        for (auto move : solution)
            std::cout << moveToString(move) << " ";
        std::cout << "\n";
    }
    return 0;
}
