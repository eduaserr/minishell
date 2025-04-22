/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:44:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/22 20:18:14 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>

typedef enum e_quote_status
{
	NO_QUOTES = 0,
	CLOSED = 1,
	UNCLOSED = -1,
	EMPTY = -2,
}	t_quote_status;

typedef struct s_env //env structure
{
	char			*key;
	char			*value;
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
/*					QUOTES					*/
/* **************************************** */
//////////////////////
//	quotes_expand	//
//////////////////////
char	*expand_var(t_shell **mshell, char **input, int i);

//////////////////////
//	quotes_check	//
//////////////////////
int		ft_check_quotes(char *input, int i);

void	parse_input(t_shell **mshell, char *input);

//////////////////////
//	quotes_utils	//
//////////////////////
int		get_quote(char *str);

char	*rm_empty_quotes(char *str, int start, int end);

char	*get_in_quotes(char *str, int start, int end);
/**
 * @brief This function processes the input string, removes the quotes (single or double),
 * and returns a newly allocated string without the quotes. The original input
 * string is freed during the process.
 * 
 * @param input Pointer to the string to process. The string is freed inside the function.
 * @param i Index of the first quote in the string.
 * @return (char *) A new string without quotes. The caller is responsible for freeing it.
 */
char	*rm_quotes(char **input, int i);

char	*rm_empty_quotes(char *str, int start, int end);

/* **************************************** */
/*					SIGNALS					*/
/* **************************************** */
//////////////////////
//	sig_init		//
//////////////////////
void	signal_function(void);

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

char	**ft_split_input(char *str);

#endif