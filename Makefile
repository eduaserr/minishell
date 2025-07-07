# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/13 15:47:17 by eduaserr          #+#    #+#              #
#    Updated: 2025/07/07 18:46:00 by aamoros-         ###   ########.fr        #
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
CMD		= src/command
INIT	= src/init
PRS		= src/parser
QUOTES	= src/quotes
SIG		= src/signals
TKN		= src/tokens
UTLS	= src/utils
EXEC	= src/executor
BINS	= src/builtins

SRC		= main.c

SRCS	= $(CMD)/cmd.c $(CMD)/get_cmd.c $(CMD)/get_args.c\
		$(INIT)/init_cmd.c $(INIT)/init_env.c $(INIT)/init_redir.c $(INIT)/init_tkn.c $(INIT)/init.c\
		$(PRS)/parser.c $(PRS)/promp.c $(PRS)/parse_utils.c\
		$(QUOTES)/quotes_expand.c $(QUOTES)/quotes_check.c $(QUOTES)/quotes_utils.c\
		$(SIG)/sig_init.c $(SIG)/signal2.c\
		$(TKN)/tokens.c\
		$(UTLS)/utils_error.c $(UTLS)/utils_expand.c $(UTLS)/utils_free.c $(UTLS)/utils_node.c $(UTLS)/utils_perror.c\
		$(UTLS)/utils_print.c $(UTLS)/utils.c\
		$(EXEC)/execute.c $(EXEC)/exec_utils.c $(EXEC)/redir.c $(EXEC)/pipes.c $(EXEC)/redir_utils.c $(EXEC)/heredoc.c\
		$(EXEC)/heredoc_utils.c\
		$(BINS)/cd.c $(BINS)/cd_utils.c $(BINS)/echo.c $(BINS)/env.c $(BINS)/env_utils.c $(BINS)/exit.c\
		$(BINS)/pwd.c $(BINS)/builtin.c
		
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