/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:44:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/03 18:16:39 by eduaserr         ###   ########.fr       */
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
# include <stdbool.h>
# include <limits.h>
# include <errno.h>
# include <wait.h>

extern volatile sig_atomic_t	g_signal_received;

typedef enum e_token_type
{
	WORD,
	SIMPLE,
	DOUBLE,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
}	t_token_type;

typedef enum e_quote_status
{
	NO_QUOTES	= 0,
	CLOSED		= 1,
	UNCLOSED	= -1,
	EMPTY		= -2,
}	t_quote_status;

typedef struct s_env
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
	t_redir				*rd;				//redirs en el input
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
	int					cmd_count;
	int					last_exit_status;	//anterior codigo de error
	int					exit_status;		//estado de la proxima salida del programa
	int					running;			//estado actual del programa
}						t_shell;

/* **************************************** */
/*					BUILTINS				*/
/* **************************************** */
bool	execute_parent_builtin(char **cmd_wargs, t_shell *shell);

void	execute_child_builtins(char **cmd_wargs, t_shell *shell);

int		builtin_echo(t_command *cmd);

int		builtin_pwd(void);

void	builtin_env(t_shell *shell, char **envp);

int		builtin_export(t_shell *shell, char **cmd);

int		builtin_unset(t_shell *shell, char **cmd);

/* **************************************** */
/*					COMMAND					*/
/* **************************************** */
//////////////////////
//	cmd				//
//////////////////////
t_command	*get_command(t_shell *mshell, t_command *cmd, char *input);

//////////////////////
//	get_cmd			//
//////////////////////
int		pipelen(t_token *tkn);

char	*process_str(t_shell *mshell, char *str);

void	get_args(t_token *tkn, t_command *cmd);

void	dup_cmd(t_shell *mshell, t_command *cmd);

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init_cmd		//
//////////////////////
t_command	*create_cmd(t_command *new);

void	addlastcmd_node(t_command **lstcmd, t_command *node);

void	sync_arr_env(t_shell *shell);
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

void	parse_redirs(t_command **cmd, t_token *tkn);

t_redir	*redir_node(t_shell *mshell, t_token *tkn, t_redir *lstrd, int cmd_index);

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
void	dollar_case(t_env *env, int e_status, char **str);

void	swp_value(char **input, char *value, int i, int end);


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

void	signal_default(void);

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
int	update_env_pwd(t_shell *shell, char *oldpwd_val);

//////////////////////
//	utils_error		//
//////////////////////
void	ft_exit(t_shell **mshell);

void	ft_exit_child(t_shell **mshell, int code);

void	ft_error(char *str);

void	ft_perror(char *str, char *var);

void	ft_error_exit(t_shell **mshell, char *message, int code);

char	*get_word_msh(char *str, int i);

//////////////////////
//	utils_expand	//
//////////////////////
int		dollar_expand(char **str, t_env *env, int i);

//////////////////////
//	utils_free		//
//////////////////////
void	ft_free_env(t_env **lstenv);

void	ft_free_tkn(t_token **tkn);

void	ft_free_cmd(t_command **cmd);

void	ft_free_mshell(t_shell **mshell);

//////////////////////
//	utils_node		//
//////////////////////
t_env	*ft_getlstenv(t_env *env, char *var);

void	sync_env_array(t_shell *shell);
/**
 * @brief You must indicate the key and it returns the corresponding value.
 * 
 * @param env content data.
 * @param var variable for being search.
 * @return (char *) A new dup string. The caller is responsible for freeing it.
 */
char	*ft_getenv(t_env *env, char *var);

char	*is_var(char *str, t_env *env);

int		ft_strcmp(const char *s1, const char *s2);

//////////////////////
//	utils_print		//
//////////////////////
void	ft_printenv(t_env *lstenv);

void	ft_printtkn(t_token *tkn);

void	ft_printcmd(t_command *cmd);

//////////////////////
//	utils			//
//////////////////////
int		ft_isredir(int a);

int		skip_quoted(char *str, int *i);

char	**ft_init_array(char **array);

void	ft_free_array(void **array);

/* **************************************** */
/*					BUILTINS				*/
/* **************************************** */
char	*get_env(char *env, char **envp);
int		unset_env(char *env, char **envp);
int		set_env(char *env, char ***envp);
int		ft_strcmp(const char *s1, const char *s2);
int		builtin_echo(t_command *cmd);
int		builtin_pwd(void);
void	builtin_env(t_shell *shell, char **envp);
int		builtin_cd(t_shell *shell, char **cmd);
int		builtin_exit(char **args);
int		builtin_unset(t_shell *shell, char **cmd);
int		builtin_export(t_shell *shell, char **cmd);

/* **************************************** */
/*					EXECUTE					*/
/* **************************************** */
char	*get_cmd_paths(char *cmd, char **env);
void	execute_heredoc(char *delimiter, int heredoc_fd[2]);
void	redirect_stdin(t_shell *shell, bool handle_heredoc);
void	setup_redirection(t_shell *shell, bool handle_heredoc);
void	exec_cmd(t_shell *shell, char **cmd_args, char **env);
void	execute(t_shell *shell, char **cmd_args, char **env);
void	handle_pipes(t_shell *shell, char **env);



#endif