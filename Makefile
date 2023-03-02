NAME = webserv

MAIN = ./src/main/
HTTP = ./src/http/
PARSER = ./parsing/
UTILS = ./src/utility/
TEST = ./src/test/

src = 	$(PARSER)request/Request.cpp \
		$(PARSER)conf/ConfigParser.cpp \
		$(PARSER)conf/Server.cpp \
		$(PARSER)conf/Http.cpp \
		$(PARSER)request/Header.cpp \
		$(UTILS)utility.cpp \
		$(TEST)socket.cpp \
		$(TEST)main.cpp

CC = c++

OBJ = $(src:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98 -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(UTILS)utility.hpp $(TEST)socket.hpp $(PARSER)request/Request.hpp $(PARSER)request/Header.hpp $(PARSER)conf/ConfigParser.hpp $(PARSER)conf/Server.hpp $(PARSER)conf/Http.hpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all