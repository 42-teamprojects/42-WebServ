NAME = webserv

SRCS = main.cpp $(wildcard src/*.cpp)

OBJS = $(SRCS:.cpp=.o)

OBJS := $(addprefix obj/, $(OBJS))

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address

INCLUDES = -I includes/

all: $(NAME)

$(NAME): $(OBJS) $(wildcard includes/*.hpp)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INCLUDES) -o $(NAME)

obj/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS)
	rm -rf -rf obj/

fclean: clean
	rm -rf $(NAME)

re: fclean all

run: all
	./$(NAME)

re-run: fclean run

.PHONY: all clean fclean re