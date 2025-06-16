/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:44:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 03:12:34 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_GENERAL_ERROR 1
# define EXIT_EXEC_FAILURE 126
# define EXIT_BUILTIN_MISUSE 2
# define SYNTAX_ERROR_STATUS 2
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131
# define EXIT_REQUEST_SIG_OFFSET 256

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

# include "../lib/libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>

extern volatile sig_atomic_t	g_signal_received;

typedef enum e_token_type
{
	WORD,
	SIMPLE,
	DOUBLE,
	PIPE,			// cuenta '|' como redirección?
	REDIR_IN,		// <
	REDIR_OUT,		// >
	APPEND,			// >>
	HEREDOC,		// <<
}	t_token_type;

/* typedef enum e_redir_status
{
	//PIPE			// cuenta '|' como redirección?
	REDIR_IN,		// <
	REDIR_OUT,		// >
	APPEND,			// >>
	HEREDOC,		// <<
}	t_redir_status;
 */
typedef enum e_quote_status
{
	NO_QUOTES	= 0,
	CLOSED		= 1,
	UNCLOSED	= -1,
	EMPTY		= -2,
}	t_quote_status;

typedef struct s_env //env structure
{
	char				*key;
	char				*value;
	struct s_env		*next;
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
	char				*cmd;				//comando a procesar
	char				**args;				//argumentos de un mismo cmd por separado
	t_redir				*redirs;			//redirs en el input
	struct s_command	*next;
}						t_command;

typedef struct s_shell
{
	char				*p_input;			//processed input
	char				*user_input;		//input orriginal del usuario
	char				**env;				//doble array de envp del sistema
	t_env				*lstenv;			//lista de las env
	t_token				*tkn;				//args separados, y divididos por tokens
	t_command			*commands;			//lista de comandos y args
	int					last_exit_status;	//anterior codigo de error
	int					exit_status;		//estado de la proxima salida del programa
	int					running;			//estado actual del programa
}						t_shell;

/* **************************************** */
/*					COMMAND					*/
/* **************************************** */
//////////////////////
//	cmd				//
//////////////////////
t_command	*get_command(t_shell *mshell, t_command *cmd, char *input);

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init_cmd		//
//////////////////////
t_command	*create_cmd(t_command *new);

void	addlastcmd_node(t_command **lstcmd, t_command *node);

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
//	init_tkn		//
//////////////////////
t_token	*get_token(t_token *new, t_token_type tkn, char *value, int *i);

void	addlast_tknnode(t_token **token_list, t_token *node);

//////////////////////
//	init			//
//////////////////////
t_shell	*init_mshell(t_shell *mshell, char **envp);

/* **************************************** */
/*					PARSER					*/
/* **************************************** */
//////////////////////
//	parser			//
//////////////////////
void	parse_input(t_shell **mshell, char *input);

//////////////////////
//	promp			//
//////////////////////
char	*promp_input(t_shell *mshell);

/* **************************************** */
/*					QUOTES					*/
/* **************************************** */
//////////////////////
//	quotes_check	//
//////////////////////
char	*check_quotes(char *input);

char 	*rm_quotes2(char *str);

/**
 * @brief This function processes the input string, and copy all character except both chars indicated
 * by index in the parameter from the function.
 * @param input Pointer to the string to process.
 * @param i Index of the first quote in the string.
 * @param j Index of the first quote in the string.
 * @return (char *) A new string without pair quotes. The caller is responsible for freeing it.
 */
char	*rm_quotes(char *input, int i, int j);
//////////////////////
//	quotes_expand	//
//////////////////////
char	*expand_var(t_shell **mshell, char **input, int i);

//////////////////////
//	quotes_utils	//
//////////////////////
int		get_quote(char *str);

char	*get_in_quotes(char *str, int start, int end);

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

//////////////////////
//	signal2			//
//////////////////////
void	setup_heredoc_signals(void);

/* **************************************** */
/*					TOKENS					*/
/* **************************************** */
//////////////////////
//	tokens			//
//////////////////////
t_token	*tokenizer(t_token *token_list, char *input);

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils_error		//
//////////////////////
void	ft_exit(t_shell **mshell);

void	ft_error(char *str);

void	ft_error_exit(t_shell **mshell, char *message, int code);

//////////////////////
//	utils_free		//
//////////////////////
void	ft_free_tkn(t_token **tkn);

void	ft_free_cmd(t_command **cmd);

void	ft_free_mshell(t_shell **mshell);

//////////////////////
//	utils_print		//
//////////////////////
void	ft_printenv(t_env *lstenv);

void	ft_printtkn(t_token *tkn);

void	ft_printcmd(t_command *cmd);

//////////////////////
//	utils_split		//
//////////////////////
char	**ft_split_input(char *str);

//////////////////////
//	utils			//
//////////////////////

int		ft_isredir(int a);

int		skip_quoted(char *str, int *i);

/**
 * @brief You must indicate the key and it returns the corresponding value.
 * 
 * @param env content data.
 * @param var variable for being search.
 * @return (char *) A new dup string. The caller is responsible for freeing it.
 */
char	*ft_getenv(t_env *env, char *var);

char	**ft_init_array(char **array);

#endif