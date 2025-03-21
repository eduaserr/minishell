#ifndef MINISHELL_H
#define MINISHELL_H

#include "../lib/libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

typedef struct s_env //env structure
{
	char	**env;
	char	**path;
}	t_env;

typedef struct s_shell
{
	int		exit_status;
	char	**user_input;
	t_env	*env;
}	t_shell;

/* **************************************** */
/*					SIGNALS					*/
/* **************************************** */
//////////////////////
//	sig_init		//
//////////////////////
void	signal_function();

t_shell	*init_mshell(t_shell *mshell, char **envp);

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils_error		//
//////////////////////
void	ft_error(char *str);

//////////////////////
//	utils_free		//
//////////////////////
void	ft_free_mshell(t_shell *mshell);

//////////////////////
//	utils			//
//////////////////////
char	**ft_init_array(char **array);

//////////////////////
//	init.c			//
//////////////////////
t_shell	*init_mshell(t_shell *mshell, char **envp);

#endif