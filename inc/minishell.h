/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 15:44:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/08 21:47:10 by eduaserr         ###   ########.fr       */
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

typedef struct s_cmd
{
	char				*cmd;
	char				**args;
	t_redir				*rd;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_shell
{
	char				*p_input;
	char				*user_input;
	char				**env;
	t_env				*lstenv;
	t_token				*tkn;
	t_cmd				*commands;
	int					cmd_count;
	int					last_exit_status;
	int					exit_status;
	int					running;
}						t_shell;

/* **************************************** */
/*					BUILTINS				*/
/* **************************************** */
//////////////////////
//	builtin.c		//
//////////////////////
bool	execute_parent_builtin(char **cmd_args, t_shell *shell);
void	execute_child_builtins(char **cmd_args, t_shell *shell);

//////////////////////
//	cd.c			//
//////////////////////
int		builtin_cd(t_shell *shell, char **cmd);

//////////////////////
//	cd_utils.c		//
//////////////////////
int		update_env_pwd(t_shell *shell, char *oldpwd_val);

//////////////////////
//	echo.c			//
//////////////////////
int		builtin_echo(t_cmd *cmd);

//////////////////////
//	env.c			//
//////////////////////
void	builtin_env(char **envp);
int		builtin_export(t_shell *shell, char **cmd);
int		builtin_unset(t_shell *shell, char **cmd);

//////////////////////
//	env_utils.c		//
//////////////////////
char	*get_env(char *env, char **envp);
int		unset_env(char *env, char **envp);
int		set_env(char *env, char ***envp);

//////////////////////
//	exit.c			//
//////////////////////
int		builtin_exit(char **args);

//////////////////////
//	pwd.c			//
//////////////////////
int		builtin_pwd(void);

/* **************************************** */
/*					COMMAND					*/
/* **************************************** */
//////////////////////
//	cmd.c			//
//////////////////////
t_cmd	*get_cmd(t_shell *shell, t_cmd *cmd, char *input, int pipe);

//////////////////////
//	get_cmd.c		//
//////////////////////
int		pipelen(t_token *tkn);
char	*process_str(t_shell *mshell, char *str);
void	dup_cmd(t_shell *mshell, t_cmd *cmd);

//////////////////////
//	get_args.c		//
//////////////////////
void	get_args(t_token *tkn, t_cmd *cmd);

/* **************************************** */
/*					EXECUTOR				*/
/* **************************************** */
//////////////////////
//	exec_utils.c	//
//////////////////////
char	*get_cmd_paths(char *cmd, char **env);

//////////////////////
//	execute.c		//
//////////////////////
void	exec_cmd(t_shell *shell, char **cmd_args, char **env);
void	execute(t_shell *shell, char **cmd_args, char **env);

//////////////////////
//	heredoc.c		//
//////////////////////
void	create_heredocs(t_shell *shell);

//////////////////////
//	heredoc_utils.c	//
//////////////////////
void	cleanup_interrupted_heredocs(t_shell *shell);
void	read_heredoc_lines(t_shell *shell, t_redir *rd, int fd, char *file);
void	clean_sigint(t_shell *shell, int status);

//////////////////////
//	pipes.c			//
//////////////////////
void	handle_pipes(t_shell *shell, char **env);

//////////////////////
//	redir_utils.c	//
//////////////////////
void	setup_redirection(t_shell *shell, bool handle_heredoc);

//////////////////////
//	redir.c			//
//////////////////////
void	redirect_stdin(t_shell *shell, bool handle_heredoc);

/* **************************************** */
/*					INIT					*/
/* **************************************** */
//////////////////////
//	init.c			//
//////////////////////
void	update_shlvl(t_env *lstenv);
t_shell	*init_mshell(t_shell *mshell, char **envp);
void	sync_arr_env(t_shell *shell);

//////////////////////
//	init_cmd.c		//
//////////////////////
int		ft_nodelen(t_token *tkn);
t_cmd	*create_cmd(t_cmd *new);
void	addlastcmd_node(t_cmd **lstcmd, t_cmd *node);

//////////////////////
//	init_env.c		//
//////////////////////
t_env	*create_env(char *env);
void	addlast_node(t_env **lstenv, t_env *node);

//////////////////////
//	init_redir.c	//
//////////////////////
t_redir	*create_redir(void);
void	addlast_redir(t_redir **lstrd, t_redir *node);
t_redir	*redir_node(t_shell *mshell, t_token *tkn, t_redir *lstrd, int cmd_ix);

//////////////////////
//	init_tkn.c		//
//////////////////////
t_token	*get_token(t_token *new, t_token_type tkn, char *value, int *i);
void	addlast_tknnode(t_token **token_list, t_token *node);

/* **************************************** */
/*					PARSER					*/
/* **************************************** */
//////////////////////
//	parser.c		//
//////////////////////
int		handle_pipes_err(char *str, int i);
int		handle_rd_err(t_shell *shell, t_token *tkn);

//////////////////////
//	parser.c		//
//////////////////////
void	parse_commands(t_shell **mshell, int tmp);
void	parse_input(t_shell **mshell, char *input);

//////////////////////
//	promp.c			//
//////////////////////
char	*promp_input(t_shell *mshell);

/* **************************************** */
/*					QUOTES					*/
/* **************************************** */
//////////////////////
//	quotes_check.c	//
//////////////////////
char	*check_quotes(t_shell *shell, char *input);

//////////////////////
//	quotes_expand.c	//
//////////////////////
void	dollar_case(t_env *env, int e_status, char **str);
int		dollar_qu(char **str, int e_status, int i);

//////////////////////
//	quotes_utils.c	//
//////////////////////
int		get_quote(char *str);
char	*rm_empty_quotes(char *str, int start, int end);
char	*rm_quotes(char *input, int i, int end);
char	*rm_quotes2(char *str);

/* **************************************** */
/*					SIGNALS					*/
/* **************************************** */
//////////////////////
//	sig_init.c		//
//////////////////////
void	signal_function(void);
void	handle_exec_sigquit(void);

//////////////////////
//	signal2.c		//
//////////////////////
void	setup_heredoc_signals(void);
void	init_signal_handler_exec(void);
void	sigint_heredoc_handler(int sig);

/* **************************************** */
/*					TOKENS					*/
/* **************************************** */
//////////////////////
//	tokens.c		//
//////////////////////
t_token	*tokenizer(t_token *token_list, char *input);

/* **************************************** */
/*					UTILS					*/
/* **************************************** */
//////////////////////
//	utils.c			//
//////////////////////
int		ft_isredir(int a);
int		skip_quoted(char *str, int *i);
char	**ft_init_array(char **array);
char	*get_word_msh(char *str, int i);
void	ft_free_array(void **array);

//////////////////////
//	utils_error.c	//
//////////////////////
void	ft_exit(t_shell **mshell);
void	ft_exit_child(t_shell **mshell, int code);

//////////////////////
//	utils_expand.c	//
//////////////////////
void	swp_value(char **input, char *value, int i, int end);
int		dollar_expand(char **str, t_env *env, int i);

//////////////////////
//	utils_free.c	//
//////////////////////
void	ft_free_env(t_env **lstenv);
void	ft_free_tkn(t_token **tkn);
void	ft_free_cmd(t_cmd **cmd);
void	ft_free_mshell(t_shell **mshell);

//////////////////////
//	utils_node.c	//
//////////////////////
t_env	*ft_getlstenv(t_env *env, char *var);
char	*ft_getenv(t_env *env, char *var);
char	*is_var(char *str, t_env *env);
void	sync_env_array(t_shell *shell);

//////////////////////
//	utils_perror.c	//
//////////////////////
void	ft_error(char *str);
void	ft_perror(t_shell *shell, char *str, char *var);

//////////////////////
//	utils_print.c	//
//////////////////////
void	ft_printtkn(t_token *tkn);
void	ft_printcmd(t_cmd *cmd);
void	ft_printenv(t_env *lstenv);

#endif