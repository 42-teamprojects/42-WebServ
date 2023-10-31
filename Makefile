NAME = webserv

SRCS = src/main.cpp

OBJS = $(SRCS:.cpp=.o)

OBJS := $(addprefix obj/, $(OBJS))

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = -I includes/

all: $(NAME)

$(NAME): $(OBJS)
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

.PHONY: all clean fclean re