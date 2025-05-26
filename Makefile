### PROGRAM ###
NAME       := minishell
CC         := clang

### LIBFT CONFIG (must come *before* CFLAGS) ###
LIB        := ./libft
LIBFT_INC  := $(LIB)/include
LIBFT_A    := $(LIB)/libft.a
LIBFT_LDFLAGS := -L$(LIB) -lft

### COMPILER FLAGS ###
CFLAGS := -Wall -Wextra -Werror -g -I. -I$(LIBFT_INC)

### LINKER FLAGS ###
READLINE_LDFLAGS := -lreadline

### UTILITIES ###
RM         := rm -f

### SOURCE FILES ###
SRC_DIRS    := builtins executor helpu redirection signals utils lexer expander parser initia
SRCS       := $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) main.c

ifeq ($(SRCS),)
  $(error No source files found in $(SRC_DIR))
endif

OBJS       := $(SRCS:.c=.o)

### PHONY TARGETS ###
.PHONY: all clean fclean re libft

### DEFAULT ###
all: libft $(NAME)

### LINK ###
$(NAME): $(OBJS)
	@echo "Linking $@..."
	$(CC) $(CFLAGS) $(OBJS) \
	      $(LIBFT_LDFLAGS) $(READLINE_LDFLAGS) \
	      -o $@
	@echo "$@ created."

### COMPILE ###
# src/foo.c → src/foo.o
$(SRC_DIRS)/%.o: $(SRC_DIRS)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

### BUILD LIBFT ###
# Only rebuilds if libft.a is missing or its Makefile says so
libft:
	@echo "Building libft..."
	@$(MAKE) -C $(LIB)

### CLEAN OBJECTS ###
clean:
	@echo "Cleaning project object files..."
	$(RM) $(OBJS)
	@echo "Cleaning libft objects..."
	@$(MAKE) -C $(LIB) clean

### FULL CLEAN (objects + executables) ###
fclean: clean
	@echo "Removing $(NAME)..."
	$(RM) $(NAME)
	@echo "Cleaning libft completely..."
	@$(MAKE) -C $(LIB) fclean

### REBUILD ###
re: fclean all