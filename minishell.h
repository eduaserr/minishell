/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 00:41:03 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 17:22:36 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# define EXIT_CMD_NOT_FOUND 127
# define EXIT_GENERAL_ERROR 1
# define EXIT_EXEC_FAILURE 126
# define EXIT_BUILTIN_MISUSE 2
# define SYNTAX_ERROR_STATUS 2
# define EXIT_SIGINT 130
# define EXIT_SIGQUIT 131
# define EXIT_REQUEST_SIG_OFFSET 256

# define PROMPT "myshell$ "

# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}								t_redir_type;

typedef struct s_redir
{
	t_redir_type				type;
	char						*value;
	int							is_value_quoted;
	int							heredoc_fd;
}								t_redir;

typedef struct s_command
{
	t_list						*args_list;
	t_list						*redir_list;
	char						**argv;
}								t_command;

typedef enum e_token_type
{
	T_WORD,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_REDIR_APPEND,
	T_REDIR_HEREDOC,
	T_PIPE,
	T_EOF,
	T_ERROR
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	int							quoted;
}								t_token;

typedef struct s_exec_ctx
{
	t_list						*cmds;
	pid_t						*pids;
	int							num_cmds;
	int							prev_read;
	int							pipe_fds[2];
	int							cmd_idx;
	char						***envp;
}								t_exec_ctx;

typedef struct s_quote_vars
{
	int							quoted;
	char						quote_char;
}								t_quote_vars;

typedef struct s_expansion_context
{
	int							last_exit_status;
	char						**envp;
}								t_expansion_context;

typedef struct s_syntax_state
{
	int							last_was_operator;
	int							expecting_word_after_redir;
}								t_syntax_state;

typedef struct s_parser_context
{
	t_list						*command_list;
	t_command					*current_cmd;
	int							last_exit_status;
	char						**envp;
	int							error_occurred;
}								t_parser_context;

typedef struct s_shell_context
{
	char						**current_envp;
	int							last_exit_status;
	int							should_run;
	int							actual_exit_code;
}								t_shell_context;

extern volatile sig_atomic_t	g_signal_received;

int								builtin_export(t_command *cmd,
									char ***envp_ptr);
int								is_builtin(char *command);
int								execute_builtin(t_command *cmd,
									char ***envp_ptr, int is_child);
int								builtin_cd(t_command *cmd, char ***envp_ptr);
int								print_exported_vars(char **envp);
int								handle_export_assignment(char *arg,
									char *eq_pos, char ***envp_ptr);
int								process_single_export_arg(char *arg,
									char ***envp_ptr);
int								set_export_var(char *name, char *value,
									char ***envp_ptr);
int								process_export_args(char **args,
									char ***envp_ptr);
int								builtin_unset(t_command *cmd, char ***envp_ptr);
int								builtin_env(t_command *cmd, char **envp);
int								builtin_exit(t_command *cmd, int is_child);
int								builtin_pwd(t_command *cmd);
int								execute(t_list *cmds, char ***envp_ptr);
bool							setup_pipe(t_exec_ctx *ctx, bool has_next);
bool							execute_command(t_exec_ctx *ctx, t_command *cmd,
									bool has_next);
void							update_pipeline_state(t_exec_ctx *ctx,
									bool has_next);
int								cleanup_pipeline(t_exec_ctx *ctx, int status);
int								wait_for_children(pid_t *pids, int count);
char							*find_command_path(char *command);
bool							is_valid_executable(char *path);
void							handle_child_process(t_command *cmd,
									char ***envp_ptr, int pipe_in_fd,
									int pipe_out_fd);
pid_t							execute_single_command(t_command *cmd,
									char ***envp_ptr, int pipe_in_fd,
									int pipe_out_fd);
char							*get_env_value(const char *var_name,
									char **envp);
char							*expand_string(char *str, int last_exit_status,
									char **envp, int quotes_type);
void							free_token(void *token_ptr);
t_token							*create_token(t_token_type type, char *value,
									int quoted);
t_list							*lexer(const char *line);
void							free_redir(void *redir_ptr);
void							free_command(void *command_ptr);
int								build_argv(t_command *cmd);
t_token							*get_token(t_list *token_node);
void							init_syntax_state(t_syntax_state *state);
t_token_type					get_token_type(t_list *token_node);
int								validate_current_token(t_list *tokens,
									t_syntax_state *state);
int								validate_operator_token(t_token *current,
									t_token *next, t_syntax_state *state);
int								validate_pipe_token(t_token *current,
									t_token_type next_type,
									t_syntax_state *state);
int								validate_redirection_token(t_token *current,
									t_token_type next_type,
									t_syntax_state *state);
int								validate_word_token(t_token *current,
									t_syntax_state *state);
int								validate_final_state(t_syntax_state *state);
int								check_syntax(t_list *tokens);
t_list							*parser(t_list *tokens, int last_exit_status,
									char **envp);
void							close_fds(int fd1, int fd2);
int								handle_heredocs(t_list *command_list,
									int last_exit_status, char **envp);
void							restore_redirections(int *original_fds);
int								apply_redirections(t_command *cmd, int pipe_in,
									int pipe_out);
void							reset_signal_handlers(void);
void							setup_heredoc_signals(void);
void							free_split_array(char **array);
void							print_error(char *command, char *message);
void							print_error_detail(char *command, char *detail,
									char *message);
int								is_valid_var_char(char c);
char							*ft_strjoin_free(char *s1, char *s2);
char							**copy_environment(char **envp);
void							free_environment(char **envp);
void							print_syntax_error(const char *token_value);
long long						ft_llatoi_strict(const char *str,
									int *error_flag);
int								process_digits(const char **str, long long *res,
									int sign, int *error_flag);
int								validate_trailing_chars(const char **str,
									int *error_flag);
int								is_valid_var_name(const char *name);
int								find_env_var_index(const char *name,
									char **envp);
int								set_env_var(char *name, char *value,
									char ***envp_ptr);
int								remove_env_var(const char *name,
									char ***envp_ptr);
int								unset_env_var(const char *name,
									char ***envp_ptr);
void							close_if_needed(int fd, int std_fd);
int								interpret_exit_status(int status);
bool							init_execution(pid_t **pids, int count,
									t_list *cmds, char **envp);
bool							try_single_builtin(t_list *cmd_node,
									char ***envp, pid_t *pids, int *status);
int								process_pipeline(t_exec_ctx *ctx);
int								exec_parent_builtin(t_command *cmd,
									char ***envp);
void							restore_fds(int original[2]);
bool							str_in_array(const char *str,
									const char **array);
char							*search_in_path_directories(char **paths,
									char *command);
char							*check_directory_path(char *dir, char *command);
char							*handle_absolute_path(char *command);
char							**get_path_directories(void);
void							run_external_command(t_command *cmd,
									char **envp);
void							setup_signal_handlers(void);
int								read_heredoc_to_pipe(t_redir *redir,
									int last_exit_status, char **envp);
void							cleanup_fds(int final_in, int final_out,
									int pipe_in, int pipe_out);
int								handle_input(t_redir *redir, int *final_in,
									int pipe_in);
int								handle_heredoc(t_redir *redir, int *final_in,
									int pipe_in);
int								handle_output(t_redir *redir, int *final_out,
									int pipe_out);
int								handle_append(t_redir *redir, int *final_out,
									int pipe_out);
void							skip_whitespace(const char **str);
int								validate_digit_presence(const char **str,
									int *error_flag);
void							parse_sign(const char **str, int *sign);
int								handle_null_input(const char *str,
									int *error_flag);
void							*ft_realloc(void *ptr, size_t size);
char							*create_env_entry_string(char *name,
									char *value);
void							update_existing_variable(char **env_array,
									int index, char *new_entry_string);
int								add_new_variable_to_env(char ***envp_ptr,
									char *new_entry_string);
int								set_env_var(char *name, char *value,
									char ***envp_ptr);
int								remove_env_var(const char *name,
									char ***envp_ptr);
int								unset_env_var(const char *name,
									char ***envp_ptr);
void							*ft_realloc(void *ptr, size_t size);
char							*ft_strj(char *s1, char *s2);
char							*remove_quotes(char *str);
t_token_type					get_redir_type(const char **line_ptr,
									char **value);
char							*dup_redir_str(const char *start, int len);
char							*dup_redir_str(const char *start, int len);
t_token_type					handle_input_redir(const char **line_ptr,
									char **value);
t_token_type					handle_output_redir(const char **line_ptr,
									char **value);
t_list							*handle_redir(const char **line_ptr,
									t_list **tokens);
t_list							*handle_pipe(const char **line_ptr,
									t_list **tokens);
const char						*find_word_end(const char *line,
									t_quote_vars *qv);
t_token							*create_word_token(char *value,
									t_quote_vars *qv, t_list **tokens);
t_list							*handle_word(const char **line_ptr,
									t_list **tokens);
t_list							*add_eof_token(t_list **tokens);
char							*handle_exit_status_var(size_t *len);
char							*extract_braced_var(char *str, size_t *len);
char							*extract_simple_var(char *str, size_t *len);
int								is_valid_var_start_char(char c);
int								is_valid_var_char_at_position(char c,
									size_t pos);
char							*extract_var_name(char *str, size_t *len);
int								validate_braced_var_name(char *str,
									size_t start, size_t end);
char							*safe_extract_var_substring(char *str,
									size_t start, size_t len);
int								is_valid_var_start_char(char c);
int								is_valid_var_char_at_position(char c,
									size_t pos);
char							*extract_var_name(char *str, size_t *len);
int								validate_braced_var_name(char *str,
									size_t start, size_t end);
char							*safe_extract_var_substring(char *str,
									size_t start, size_t len);
char							*extract_content_from_quotes(char *str,
									int quotes_type);
char							*process_variable_expansion(char *content,
									int last_exit_status, char **envp);
char							*append_literal_text(char *result,
									char *content, size_t start, size_t end);
size_t							process_dollar_expansion(char *content,
									char **result, size_t pos,
									t_expansion_context *ctx);
char							*get_variable_value(char *var_name,
									int last_exit_status, char **envp);
char							*expand_string(char *str, int last_exit_status,
									char **envp, int quotes_type);
int								init_parser_context(t_parser_context *ctx,
									int last_exit_status, char **envp);
void							cleanup_parser_context(t_parser_context *ctx);
int								process_token(t_parser_context *ctx,
									t_list *tokens);
int								handle_pipe_token(t_parser_context *ctx,
									t_list *tokens);
t_token							*get_next_token(t_list *tokens);
int								add_command_to_list(t_parser_context *ctx);
int								create_new_current_cmd(t_parser_context *ctx);
int								handle_redirection_token(t_parser_context *ctx,
									t_list *tokens);
int								validate_redirection_syntax(t_token *token,
									t_token *next_token);
t_redir							*create_redirection(t_token *token,
									t_token *next_token, t_parser_context *ctx);
int								add_redirection_to_command(t_command *cmd,
									t_redir *redir);
int								is_operator_token(t_token_type type);
int								is_redirection_token(t_token_type type);
int								handle_word_token(t_parser_context *ctx,
									t_list *tokens);
int								add_argument_to_command(t_command *cmd,
									char *arg);
int								detect_quote_type(t_token *token);
int								handle_error_token(t_parser_context *ctx,
									t_list *tokens);
t_list							*advance_token_pointer(t_list *tokens,
									t_parser_context *ctx);
t_list							*finalize_parser(t_parser_context *ctx);
t_list							*parser(t_list *tokens, int last_exit_status,
									char **envp);
int								initialize_shell(t_shell_context *ctx,
									char **initial_envp);
int								is_exit_request(int exit_status);
void							handle_exit_request(t_shell_context *ctx,
									int exit_status);
int								cleanup_and_exit(t_shell_context *ctx);
t_list							*tokenize_input(char *line);
t_list							*parse_tokens(t_list *tokens,
									t_shell_context *ctx);
void							handle_eof_input(t_shell_context *ctx);
int								is_empty_line(char *line);
void							handle_empty_line(t_shell_context *ctx,
									char *line);
void							run_shell_loop(t_shell_context *ctx);
char							*get_user_input(t_shell_context *ctx);
void							process_command_line(t_shell_context *ctx,
									char *line);
void							execute_commands(t_shell_context *ctx,
									t_list *command_list);

#endif
