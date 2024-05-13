NAME        = ircserv

COMPILER    = g++ -c
LINKER      = g++
CFLAGS      = -Wall -Werror -Wextra -std=c++98 -g3

SRCS        = $(wildcard src/*/*.cpp)
INCS        = $(wildcard src/*/*.hpp)
OBJS        = $(SRCS:.cpp=.o)

CLEANUP     = rm -rf

all: 		$(NAME)

$(NAME): 	$(OBJS) $(INCS)
		 	$(LINKER) $(OBJS) -o $@

%.o: 		%.cpp
	 		$(COMPILER) $(CFLAGS) $< -o $@

clean:
			$(CLEANUP) $(OBJS)

fclean:		clean
			$(CLEANUP) $(NAME)

re: 		fclean all

.PHONY: 	all clean fclean re
