NAME = webserv

MAIN = ./src/main/
HTTP = ./src/http/
PARSER = ./src/parser/
UTILS = ./src/utility/
TEST = ./src/test/

src = 	$(UTILS)utility.cpp \
		$(MAIN)webserv.cpp \
		$(TEST)main.cpp

CC = c++

OBJ = $(src:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC)  $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.c $(MAIN)webserv.hpp $(UTILS)utility.hpp
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all