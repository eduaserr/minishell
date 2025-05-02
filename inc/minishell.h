/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:44:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/02 20:00:07 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>

typedef enum e_token_type
{
	WORD,
	DOUBLE,
	SIMPLE,
	REDIR,
	PIPE,
}	t_token_type;

typedef enum e_redir_status
{
	REDIR_IN,		// <
	REDIR_OUT,		// >
	APPEND,			// >>
	HEREDOC,		// <<
}	t_redir_status;

typedef enum e_quote_status
{
	NO_QUOTES = 0,
	CLOSED = 1,
	UNCLOSED = -1,
	EMPTY = -2,
}	t_quote_status;

typedef struct s_env //env structure
{
	char				*key;
	char				*value;
	struct s_env		*next;
	struct s_env		*prev;
}						t_env;

typedef struct s_token
{
	int					type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	int					type;
	char				*file;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				*input;
	char				**user_input;
	char				**env;
	t_env				*lstenv;
	t_token				*tokens;
	t_command			*commands;
	int					exit_status;
	int					running;
}						t_shell;

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init_env		//
//////////////////////
/**
 * @brief creates env variables struct. Asing the correspondign key and value.
 * If there is no value, ft_strdup("");
 * @return (t_env) returns new node.
 */
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
void	parse_input(t_shell **mshell, char *input);

//////////////////////
//	quotes_utils	//
//////////////////////
int		get_quote(char *str);

char	*get_in_quotes(char *str, int start, int end);

/**
 * @brief This function processes the input string, removes the quotes (single or double),
 * and returns a newly allocated string without the quotes. The original input
 * string is freed during the process.
 * 
 * @param input Pointer to the string to process.
 * @param i Index of the first quote in the string.
 * @return (char *) A new string without quotes. The caller is responsible for freeing it.
 */
char	*rm_quotes(char **input, int i);

/**
 * @brief This function processes the input string, removes the empty quotes (single or double),
 * and returns a newly allocated string without them.
 * 
 * @param input Pointer to the string to process. The string is freed inside the function.
 * @param start The index of the first quote.
 * @param end The index of the final quote.
 * @return (char *) A new string without quotes. The caller is responsible for freeing it.
 */
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
/**
 * @brief You must indicate the key and it returns the corresponding value.
 * 
 * @param env content data.
 * @param var variable for being search.
 * @return (char *) A new dup string. The caller is responsible for freeing it.
 */
char	*ft_getenv(t_env *env, char *var);

t_env	*split_env(t_env *new, char *env);

char	**ft_init_array(char **array);

void	ft_printenv(t_env *lstenv);

char	**ft_split_input(char *str);

#endif