NAME = npuzzle
SRCS1 := $(shell find srcs -name '*.cpp')
HDRS1 := $(shell find srcs -name '*.hpp')
OBJS1 := $(SRCS1:.cpp=.o)
# CFLAGS = -Wall -Werror -Wextra -funroll-loops -O3
CFLAGS =  -funroll-loops -O3 -std=c++17

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
	./$(NAME) 0 ./grids/solvable-3.txt

test4: all
	./$(NAME) 0 ./grids/solvable-4.txt

.PHONY: clean fclean re test test4
