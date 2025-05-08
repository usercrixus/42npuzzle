SRCS1 := $(shell find srcs -name '*.cpp') srcs/main.cpp
OBJS1 := $(SRCS1:.cpp=.o)


main.out: $(OBJS1)
	c++ -Wall -Werror -Wextra $^ -o $@

%.o: %.cpp
	c++ -c $< -o $@

clean:
	rm -f $(OBJS1)

fclean: clean
	rm -f main.out