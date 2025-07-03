NAME = npuzzle
SRCS1 := $(shell find srcs -name '*.cpp')
HDRS1 := $(shell find srcs -name '*.hpp')
OBJS1 := $(SRCS1:.cpp=.o)
CFLAGS = -Wall -Werror -Wextra -funroll-loops -O3 -std=c++17 -g3

all: $(NAME)

$(NAME): $(OBJS1) $(HDRS1)
	c++ $(CFLAGS) $(OBJS1) -o $@

%.o: %.cpp
	c++  $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS1)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: all
	./npuzzle 0 grids/solvable-3.txt
	./npuzzle -u 0 grids/solvable-3.txt
	./npuzzle -g 0 grids/solvable-3.txt

vtest: all
	valgrind ./npuzzle 0 grids/solvable-3.txt

test3: all
	time ./$(NAME) 0 ./grids/solvable-3.txt

test4: all
	time ./$(NAME) 0 ./grids/solvable-4.txt

checker:
	./npuzzle -q 0 ./grids/solvable-4.txt | python3 ./checker/checker.py grids/solvable-4.txt

.PHONY: clean fclean re test test4 checker
