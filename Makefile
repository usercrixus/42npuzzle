SRCS1 := $(shell find srcs -name '*.cpp')
HDRS1 := $(shell find srcs -name '*.hpp')
OBJS1 := $(SRCS1:.cpp=.o)

all: main.out

main.out: $(OBJS1) $(HDRS1)
	c++ -Wall -Werror -Wextra -funroll-loops -O3 $(OBJS1) -o $@

%.o: %.cpp
	c++  -Wall -Werror -Wextra -funroll-loops -O3 -c $< -o $@

clean:
	rm -f $(OBJS1)

fclean: clean
	rm -f main.out

re: fclean all

.PHONY: clean fclean re