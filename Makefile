# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ssuchane <ssuchane@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/03 15:54:07 by ssuchane          #+#    #+#              #
#    Updated: 2024/10/17 21:22:02 by ssuchane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += -s
NAME = philo
CC = cc
FLAGS = -Wall -Wextra -Werror -g -pthread
RM = rm -f

SRCS =	init.c \
		monitor.c \
		philo.c \
		protect_data.c \
		routine.c \
		utils.c \
		validate_input.c
		
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	rm -f philo
	$(CC) $(FLAGS) $(OBJS) -o $@
	$(MAKE) clean

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re