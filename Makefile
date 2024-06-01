# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/11 23:11:31 by gasouza           #+#    #+#              #
#    Updated: 2024/06/01 16:04:26 by gasouza          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= ircserv

COMPILER	= g++ -c
LINKER		= g++
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -g

SRCS		= $(wildcard src/*/*.cpp)
INCS 		= $(wildcard src/*/*.hpp)
OBJS		= $(SRCS:.cpp=.o)
MAIN		= src/main.cpp

CLEANUP		= rm -rf

all: $(NAME)

run: $(NAME)
	@./$(NAME) 8080 password

val: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 8080 password

$(NAME): $(OBJS) $(INCS) $(MAIN)
	$(LINKER) $(OBJS) $(MAIN) -o $@

%.o: %.cpp
	$(COMPILER) $(CFLAGS) $< -o $@

clean:
	$(CLEANUP) $(OBJS)

fclean: clean
	$(CLEANUP) $(NAME)

re: fclean all

.PHONY: all val run clean fclean re

## == Tests ========================= ##

TEST_NAME	= runTests

TEST_SRCS	= $(wildcard test/*/*.cpp)
TEST_OBJS	= $(TEST_SRCS:.cpp=.o)
TEST_MAIN	= test/main.cpp

test: $(TEST_NAME)

test-run: $(TEST_NAME)
	@./$(TEST_NAME)

test-run-val: $(TEST_NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TEST_NAME)

$(TEST_NAME): $(OBJS) $(INCS) $(TEST_OBJS) $(TEST_MAIN)
	$(LINKER) $(OBJS) $(TEST_OBJS) $(TEST_MAIN) -o runTests -lgtest -lgmock -lgtest_main -pthread

%Test.o: %Test.cpp
	$(COMPILER) -Wall -Werror -Wextra $< -o $@

test-clean:
	$(CLEANUP) $(TEST_OBJS)

test-fclean: test-clean
	$(CLEANUP) $(TEST_NAME)

test-re: test-fclean test

.PHONY: test test-run test-clean test-fclean test-re