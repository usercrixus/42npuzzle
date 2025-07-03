class Puzzle:
    actions_to_move = {
        "LEFT": (0, -1),
        "RIGHT": (0, 1),
        "TOP": (-1, 0),
        "BOTTOM": (1, 0),
    }
    actions = ["LEFT", "RIGHT", "TOP", "BOTTOM"]

    def __init__(
        self,
        grid: tuple,
        row: int,
        col: int,
        parent=None,
        parent_dir: str | None = None,
    ) -> None:
        self.grid: tuple = grid
        self.size: int = len(grid)
        self.row: int = row
        self.col: int = col
        self.g: float = 0
        self.f: float = 0
        self.parent: Puzzle | None = parent
        self.parent_dir: str | None = parent_dir

    def __str__(self) -> str:
        ret = ""
        for row in self.grid:
            for cell in row:
                ret += str(cell)
                if cell == row[-1]:
                    continue
                if cell < 10:
                    ret += "  "
                else:
                    ret += " "
            if row != self.grid[-1]:
                ret += "\n"
        return ret

    def __hash__(self) -> int:
        return hash(self.grid)

    def __eq__(self, other):
        return self.grid == other.grid

    def __lt__(self, other):
        return self.f < other.f

    @classmethod
    def execute(cls, src, action: str):
        new_row = src.row + Puzzle.actions_to_move[action][0]
        new_col = src.col + Puzzle.actions_to_move[action][1]
        if not -1 < new_row < src.size:
            return None, 0
        if not -1 < new_col < src.size:
            return None, 0

        new_grid = [list(row) for row in src.grid]
        new_grid[src.row][src.col], new_grid[new_row][new_col] = (
            new_grid[new_row][new_col],
            new_grid[src.row][src.col],
        )
        new_grid = tuple(tuple(row) for row in new_grid)
        return cls(new_grid, new_row, new_col, src, action), new_grid[src.row][
            src.col
        ]

    @staticmethod
    def next_snail(row, col, dir_row, dir_col, start, end):
        if col == end and dir_col == 1:
            dir_row = 1
            dir_col = 0
        elif row == end and dir_row == 1:
            dir_row = 0
            dir_col = -1
            end -= 1
        elif col == start and dir_col == -1:
            dir_row = -1
            dir_col = 0
            start += 1
        elif row == start and dir_row == -1:
            dir_row = 0
            dir_col = 1
        return row + dir_row, col + dir_col, dir_row, dir_col, start, end

    @classmethod
    def get_goal(cls, size: int) -> tuple:
        grid = [[0 for _ in range(size)] for _ in range(size)]
        col = 0
        row = 0
        dir_row = 0
        dir_col = 1
        end = size - 1
        start = 0
        goal_list = [(-1, -1)]
        for i in range(1, size**2):
            goal_list.append((row, col))
            grid[row][col] = int(i)
            row, col, dir_row, dir_col, start, end = Puzzle.next_snail(
                row, col, dir_row, dir_col, start, end
            )
        grid = tuple(tuple(row) for row in grid)
        goal = cls(grid, size - 1, size - 1)
        return goal, goal_list

    @staticmethod
    def get_start(grid) -> tuple[int, int]:
        for row, line in enumerate(grid):
            for col, n in enumerate(line):
                if n == 0:
                    return row, col
        raise OverflowError("start not found")
