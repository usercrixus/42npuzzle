import heapq
from heuristic import Manhattan, Heuristic

from puzzle import Puzzle


class Solver:
    def __init__(
        self,
        start: Puzzle,
        heuristic: Heuristic = Manhattan(),
        quiet: bool = False,
    ) -> None:
        self.size = len(start.grid)
        self.goal, self.goal_list = Puzzle.get_goal(self.size)
        self.open = [start]
        self.closed = set()
        self.start = start
        self.heuristic: Heuristic = heuristic
        self.start.g = 0
        self.start.f = self.heuristic(self.start, self.goal_list)
        self.tot_open = 0
        self.max_open = 0
        self.quiet = quiet

    def solve(self) -> None:
        while self.open:
            current = heapq.heappop(self.open)
            if current == self.goal:
                self.print_solution(current)
                return
            self.closed.add(current)
            neighbours = [
                Puzzle.execute(current, action)
                for action in Puzzle.actions_to_move.keys()
            ]
            for neighbor, _ in neighbours:
                if not neighbor or neighbor in self.closed:
                    continue

                self.tot_open += 1
                self.max_open = max(self.max_open, len(self.open))

                neighbor.g = current.g + 1
                neighbor.f = neighbor.g + self.heuristic(
                    neighbor, self.goal_list
                )
                heapq.heappush(self.open, neighbor)
        print("The puzzle in not solvable")

    def print_solution(self, node: Puzzle):
        cur = node
        solution = []
        while cur:
            solution.insert(0, cur.parent_dir)
            cur = cur.parent
        for move in solution[1:]:
            print(move, end=" ")
        print("")
        if not self.quiet:
            print("Number of move:", len(solution) - 1)
            print("Total selected states:", self.tot_open)
            print("Maximum selected states:", self.max_open)
