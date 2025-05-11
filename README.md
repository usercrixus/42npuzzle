# 42 N-Puzzle Solver

A C++ implementation of the classic N-Puzzle solver using the A* search algorithm, developed for the 42 School curriculum.

## 📌 Project Description

This project solves the N-Puzzle problem (e.g., 3x3, 4x4, etc.) using the A* search algorithm with a variety of heuristics. The program parses puzzle inputs from text files, validates them (including solvability), and then finds an optimal sequence of moves to reach the goal configuration.

## 🧠 Features

- A* algorithm implementation
- Support for different puzzle sizes (3x3, 4x4, 5x5, etc.)
- Solvability check
- Multiple predefined puzzle files
- Puzzle generator (`npuzzle-gen.py`)

## 🛠 Build Instructions

    make # This will compile the project and generate the executable main.out
    make re # Delete all and rebuild
    make clean # Delete *.o
    make fclean # Delete main.out + clean

## 🚀 Usage

    ./main.out <heuristic_mode> [path_to_puzzle_file]

Example:

    ./main.out 2 npuzzle-grid/npuzzle-3-hard.txt

Use the Python generator to create your own grid:

python3 npuzzle-grid/npuzzle-gen.py -s 3 > new_puzzle.txt
