SHELL := /bin/zsh
NAME = npuzzle
SRCS1 := $(shell find srcs -name '*.cpp')
HDRS1 := $(shell find srcs -name '*.hpp')
OBJS1 := $(SRCS1:.cpp=.o)
CFLAGS = -Wall -Werror -Wextra -funroll-loops -O3 -std=c++17 -g3
CC = clang++

all: $(NAME)

$(NAME): $(OBJS1) $(HDRS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $@

%.o: %.cpp
	$(CC)  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1)

fclean: clean
	rm -f $(NAME)

re: fclean all

u:all
	python3 grids/npuzzle-gen.py -u 3 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -u 4 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -u 5 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -u 6 > g && ./npuzzle -q -g 0 g
s:all
	python3 grids/npuzzle-gen.py -s 3 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -s 4 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -s 5 > g && ./npuzzle -q -g 0 g
	python3 grids/npuzzle-gen.py -s 6 > g && ./npuzzle -q -g 0 g


test: all
	./npuzzle 0 grids/solvable-3.txt
	./npuzzle -u 0 grids/solvable-3.txt
	./npuzzle -g 0 grids/solvable-3.txt

testig: all
	./npuzzle -g 0 grids/solvable-3.txt
	./npuzzle -g 1 grids/solvable-3.txt
	./npuzzle -g 2 grids/solvable-3.txt

testh: all
	./npuzzle 0 grids/solvable-3.txt
	./npuzzle 1 grids/solvable-3.txt
	./npuzzle 2 grids/solvable-3.txt


vtest: all
	valgrind ./npuzzle 0 grids/solvable-3.txt

test3: all
	time ./$(NAME) 0 ./grids/solvable-3.txt

test4: all
	time ./$(NAME) 0 ./grids/solvable-4.txt

checker:
	./npuzzle -q 0 ./grids/solvable-4.txt | python3 ./checker/checker.py grids/solvable-4.txt

.PHONY: clean fclean re test test4 checker
