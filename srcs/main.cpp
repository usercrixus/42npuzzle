#include "AStarSolver.hpp"
#include "Heuristic.hpp"
#include "NPuzzle.hpp"
#include "parser/arguments.hpp"

#include <exception>
#include <iostream>
#include <ostream>

int main(int argc, char const *argv[])
{
	options opts;
	(void)argc;
	try
	{
		opts = parse_args(argv);
		NPuzzle puzzle = opts.generatePuzzle ? NPuzzle(4) : NPuzzle(opts.puzzlePath);

		if (!opts.printMovesOnly)
		{
			std::cout << "===== Initial puzzle =====" << std::endl;
			puzzle.print();
		}

		if (puzzle.isSolvable())
			return std::cout << "This puzzle is unsolvable (odd inversion count)." << std::endl, 0;

		AStarSolver s(opts, puzzle);
		s.solve();
		if (!opts.printMovesOnly)
			s.printInfo();
		s.printSolution();
		return 0;
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Usage: " << argv[0] << " [-q] [-u] <heuristicMode> <puzzle_file>" << std::endl;
		return 1;
	}
}
