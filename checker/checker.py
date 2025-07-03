import sys

from parsing import pars_args, pars_grid
from puzzle import Puzzle


def main() -> int:
    args = pars_args()
    grid = pars_grid(args.file)
    row, col = Puzzle.get_start(grid)
    goal, _ = Puzzle.get_goal(len(grid))
    puzzle = Puzzle(grid, row, col)
    moves = sys.stdin.read().split()
    for move in moves:
        puzzle, _ = Puzzle.execute(puzzle, move)
    print(puzzle == goal)
    return 0

if __name__ == "__main__":
    try:
        main()
    except Exception as _:
        print(False)
