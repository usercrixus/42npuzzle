#pragma once

#include <string>

typedef struct s_options
{
	bool		uniform = false;
	bool		greedy = false;
	bool		printMovesOnly = false;
	int			heuristicMode = false;
	bool		generatePuzzle = false;
	std::string puzzlePath = std::string();
} options;

options parse_args(const char *argv[]);
