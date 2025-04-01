#ifndef MINISHELL_H
#define MINISHELL_H

#include "../lib/libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <stdio.h>

typedef struct s_env //env structure
{
	char	*key;
	char	*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_shell
{
	char	**user_input;
	char	**env;
	t_env	*lstenv;
}	t_shell;

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init_env		//
//////////////////////
t_env	*create_env(char *env);

void	addlast_node(t_env **lstenv, t_env *node);

//////////////////////
//	init.c			//
//////////////////////
t_shell	*init_mshell(t_shell *mshell, char **envp);

/* **************************************** */
/*					SIGNALS					*/
/* **************************************** */
//////////////////////
//	sig_init		//
//////////////////////
void	signal_function();

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils_error		//
//////////////////////
void	ft_exit(t_shell **mshell);

void	ft_error(char *str);

//////////////////////
//	utils_free		//
//////////////////////
void	ft_free_mshell(t_shell **mshell);

//////////////////////
//	utils			//
//////////////////////
char	*ft_getenv(t_env *env, char *var);

t_env	*split_env(t_env *new, char *env);

char	**ft_init_array(char **array);

void	ft_printenv(t_env *lstenv);

#endif