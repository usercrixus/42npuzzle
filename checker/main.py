from parsing import pars_args, pars_grid
from heuristic import heuristics
from solver import Solver
from puzzle import Puzzle


def main():
    args = pars_args()
    grid = pars_grid(args.file)
    row, col = Puzzle.get_start(grid)
    puzzle = Puzzle(grid, row, col)
    solver = Solver(puzzle, heuristic=heuristics[args.d](), quiet=args.quiet)
    solver.solve()


if __name__ == "__main__":
    # try:
    main()
# except Exception as error:
# print(type(error).__name__ + ":", error)
