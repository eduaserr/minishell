NAME	=	libft.a
AR		=	ar -rcs
CC		=	clang # Use clang as specified, removing -O3 for debugging if needed
FLAGS	=	-Wall -Werror -Wextra -g # Added -g for easier debugging

#directories
SRC_DIR					=	src/
OBJ_DIR					=	obj/
INC_DIR					=	include/

#controll codes (optional, kept for consistency)
RESET			=	\033[0m
GREEN			=	\033[32m
YELLOW			=	\033[33m
# ... other colors ...
UP				=	\033[A
CUT				=	\033[K

#source files (Combine standard and bonus)
SRC_FILES = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
	ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atod.c ft_atof.c \
	ft_atoi.c ft_atol.c ft_calloc.c ft_strdup.c ft_putchar_fd.c ft_putstr_fd.c \
	ft_putendl_fd.c ft_putnbr_fd.c ft_striteri.c ft_strmapi.c ft_substr.c \
	ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c get_next_line.c \
	get_next_line_utils.c ft_printf.c ft_printf_utils.c ft_printf_utils2.c \
	ft_itoa_printf.c ft_strcmp.c ft_strstr.c

SRCBON_FILES	=	ft_lstnew_bonus.c \
					ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
					ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

# Combine source lists
SOURCES = $(SRC_FILES) $(SRCBON_FILES)

# Object files derived from the combined list
OBJ_FILES	=	$(SOURCES:.c=.o)

#paths
SRC 		=	$(addprefix $(SRC_DIR), $(SOURCES))
OBJ 		=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

# Default target builds everything including bonus
all: $(NAME)

#compile the library archive
$(NAME): $(OBJ)
	@echo "$(YELLOW)Archiving [$(NAME)]...$(RESET)"
	@$(AR) $(NAME) $(OBJ)
	@echo "$(GREEN)Finished Archive [$(NAME)]$(RESET)"

#compile object files
$(OBJ_DIR)%.o:$(SRC_DIR)%.c include/libft.h Makefile # Depend on header and Makefile
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling [$<]...$(RESET)"
	@$(CC) $(FLAGS) -I $(INC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished [$<]$(RESET)"
	@printf "$(UP)$(CUT)"

# Bonus rule is now redundant if 'all' includes everything, but can be kept as an alias
bonus: all
	@echo "Libft bonus functions included in default build."

#clean rule
clean:
	@echo "$(YELLOW)Cleaning libft objects...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@echo "$(GREEN)Libft objects cleaned.$(RESET)"

#fclean rule
fclean: clean
	@echo "$(YELLOW)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)$(NAME) cleaned.$(RESET)"

#re rule
re: fclean all

#phony
.PHONY: all bonus clean fclean re

