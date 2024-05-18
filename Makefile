# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gasouza <gasouza@student.42sp.org.br >     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/11 23:11:31 by gasouza           #+#    #+#              #
#    Updated: 2024/05/15 20:14:08 by gasouza          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= ircserv

COMPILER	= g++ -c
LINKER		= g++
CFLAGS		= -Wall -Werror -Wextra -std=c++98

SRCS		= $(wildcard src/*/*.cpp)
INCS 		= $(wildcard src/*/*.hpp)
OBJS		= $(SRCS:.cpp=.o)

CLEANUP		= rm -rf

all: $(NAME)

$(NAME): $(OBJS) $(INCS)
	$(LINKER) $(OBJS) -o $@

%.o: %.cpp
	$(COMPILER) $(CFLAGS) $< -o $@

clean:
	$(CLEANUP) $(OBJS)

fclean: clean
	$(CLEANUP) $(NAME)

re: fclean all

.PHONY: all clean fclean re
