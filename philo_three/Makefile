# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: jbennink <jbennink@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2020/11/16 09:21:05 by jbennink      #+#    #+#                  #
#    Updated: 2020/11/16 09:21:05 by jbennink      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = philo_three

SRCS = ft_helper_functions.c \
	   locked_writing.c \
	   main.c \
	   philo_loop.c \
	   setup.c

OBJS = $(SRCS:.c=.o)

INCL = philosophers.h

FLAGS = -Wall -Wextra -Werror

OS = $(shell uname)
ifeq ($(OS),Linux)
	FLAGS += -pthread
endif

all: $(NAME)

%.o: %.c $(INCL)
	gcc $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc $(FLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
