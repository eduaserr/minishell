#ifndef MINISHELL_H
#define MINISHELL_H

#include "../lib/libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>

typedef struct s_fd
{
	int	in;
	int	out;
}	t_fd;

typedef struct s_env //env structure
{
	char	**env;
	char	**path;
}	t_env;

typedef struct s_shell
{
	char	**args;
}	t_shell;

typedef struct s_stack
{
	int		exit_status;
	char	**user_input;
	t_shell	*minishell;
	t_env	*env;
}	t_stack;

#endif