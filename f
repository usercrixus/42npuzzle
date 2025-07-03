c++  -Wall -Werror -Wextra -funroll-loops -O3 -std=c++17 -c srcs/ANode.cpp -o srcs/ANode.o
c++ -Wall -Werror -Wextra -funroll-loops -O3 -std=c++17 srcs/AStarSolver.o srcs/ANode.o srcs/NPuzzle.o srcs/Heuristic.o srcs/main.o srcs/parser/arguments.o -o npuzzle
./npuzzle 0 grids/solvable-3.txt
===== Initial puzzle =====
5 3 1 
7 8 6 
4 2 0 

===== Summary =====
Solution moves (25):
Number of state selected (1480):
Max number of state in memory (568):
Solution:
êÅîo TOP TOP LEFT BOTTOM BOTTOM LEFT TOP TOP RIGHT BOTTOM RIGHT TOP LEFT BOTTOM LEFT TOP RIGHT RIGHT BOTTOM LEFT BOTTOM RIGHT TOP LEFT 
# ./npuzzle -u 0 grids/solvable-3.txt
# ./npuzzle -g 0 grids/solvable-3.txt
