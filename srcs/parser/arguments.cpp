#include "arguments.hpp"
#include <stdexcept>

static void parse_option(const std::string arg, options &opts)
{
	for (auto letter : arg.substr(1))
	{
		switch (letter)
		{
		case 'q':
			opts.printMovesOnly = true;
			break;
		case 'u':
			opts.uniform = true;
			break;
		case 'g':
			opts.greedy = true;
			break;
		default:
			throw std::runtime_error(std::string("unknown options: ") + arg);
		}
	}
}

options parse_args(const char *argv[])
{
	argv++;
	options opts = {};
	while (*argv && *argv[0] == '-')
	{
		parse_option(*argv, opts);
		argv++;
	}
	if (opts.greedy && opts.uniform)
		throw std::runtime_error("option -u and -g are not compatible");
	if (!*argv)
		throw std::runtime_error("missing heuristicMode");
	opts.heuristicMode = atoi(*argv);
	argv++;
	if (!*argv)
		return opts.generatePuzzle = true, opts;
	opts.puzzlePath = *argv;
	argv++;
	if (*argv)
		throw std::runtime_error("too many arguements");
	return opts;
}
