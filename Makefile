# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 15:47:17 by eduaserr          #+#    #+#              #
#    Updated: 2025/04/15 18:58:32 by eduaserr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### COMPILATION ###
NAME	= minishell
CC	= clang
CFLAGS	= -Wall -Wextra -Werror -g
MINISHELL = -lreadline
LIB		:= ./lib/libft
LIBS	:= -L $(LIB) -lft
MAKE	= make --no-print-directory
RM		= rm -f

### SRCS ###
INIT	= src/init
SIG		= src/signals
UTLS	= src/utils
QUOTES	= src/quotes
SRC		= main.c

SRCS	= $(INIT)/init_env.c $(INIT)/init.c\
		$(QUOTES)/quotes_expand.c $(QUOTES)/quotes_check.c $(QUOTES)/quotes_utils.c\
		$(SIG)/sig_init.c\
		$(UTLS)/utils_error.c $(UTLS)/utils_free.c $(UTLS)/utils.c

### OBJS ###
OBJS	= $(SRC:.c=.o) $(SRCS:.c=.o)

### RULES ###
all : libft $(NAME)

$(NAME): $(OBJS)
	@echo "loading minishell..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME) $(MINISHELL)
	@echo "minishell compiled successfully"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

libft:
	@$(MAKE) -C $(LIB)

clean:
	@echo "clearing minishell...🧹"
	@$(RM) $(OBJS) main.o
	@echo "clearing libft...🧹"
	@$(MAKE) -C $(LIB) clean

fclean: clean
	@$(RM) $(NAME) main
	@$(MAKE) -C $(LIB) fclean
	@echo "clearing minishell executable"

re: fclean all

.PHONY: all clean fclean re libft