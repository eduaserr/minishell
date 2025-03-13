# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 15:47:17 by eduaserr          #+#    #+#              #
#    Updated: 2025/03/13 16:52:32 by eduaserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COMPILATION ###
NAME	= minishell
CC	= clang
CFLAGS	= -Wall -Wextra -Werror -g
MINISHELL = -lreadline
LIB		:= ./lib/libft
LIBS	:= -L $(LIB) -lft
RM		= rm -f

### SRCS ###
SRC		= main.c
SRCS	=

### OBJS ###
OBJS	= $(SRC:.c=.o) $(SRCS:.c=.o)

### RULES ###
all : libft $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(MINISHELL)
	@echo "minishell compiled successfully"

libft:
	@$(MAKE) -C $(LIB)

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS) main.o
	@$(MAKE) -C $(LIB) clean

fclean: clean
	@$(RM) $(NAME) main
	@$(MAKE) -C $(LIB) fclean

re: fclean all

.PHONY: all clean fclean re libft