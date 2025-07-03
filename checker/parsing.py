import argparse


def pars_args():
    parser = argparse.ArgumentParser(
        prog="NPuzzle", description="42 NPuzzle A* star solver"
    )
    parser.add_argument("file")
    parser.add_argument(
        "-d",
        type=int,
        default=0,
        choices=[0, 1, 2],
        help="0:Manhattan, 1: Euclidean, 2:Misplaced",
    )
    parser.add_argument(
        "-q", "--quiet", action="store_true", help="Only displays the solution"
    )
    return parser.parse_args()


def pars_grid(path: str):
    file = open(path, "r")
    grid = []
    for i, line in enumerate(file):
        if i != 0:
            nums = line[:-1].split(" ")
            nums = tuple(int(n) for n in nums if n != "")
            grid.append(nums)
    return tuple(grid)
