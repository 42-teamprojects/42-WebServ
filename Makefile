NAME = webserv

SRCS = main.cpp \
       src/Cgi.cpp \
       src/Client.cpp \
       src/Config.cpp \
       src/Request.cpp \
       src/Response.cpp \
       src/ResponseUtils.cpp \
       src/Route.cpp \
       src/Server.cpp \
       src/WebServer.cpp \
	   src/addons/utils.cpp \
	   src/addons/helpers.cpp \
	   src/addons/ServerException.cpp \
	   src/addons/Mimes.cpp \

OBJS = $(SRCS:.cpp=.o)

HEADERS = includes/Cgi.hpp \
		includes/Client.hpp \
		includes/Config.hpp \
		includes/Console.hpp \
		includes/enums.hpp \
		includes/Request.hpp \
		includes/Response.hpp \
		includes/Route.hpp \
		includes/Server.hpp \
		includes/WebServer.hpp \
		includes/webserv.hpp \
		includes/addons/utils.hpp \
		includes/addons/helpers.hpp \
		includes/addons/ServerException.hpp \
		includes/addons/Mimes.hpp \

OBJS := $(addprefix obj/, $(OBJS))

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

INCLUDES = -I includes/ -I includes/addons/

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
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

run:
	python3 watcher.py $(NAME)

re-run: fclean run

.PHONY: all clean fclean re