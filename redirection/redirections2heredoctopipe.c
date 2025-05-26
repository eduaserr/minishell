/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2heredoctopipe.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:19:37 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/07 17:06:19 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Read heredoc input until delimiter is found, write to a pipe
// Helper for variable expansion
static int	expand_heredoc_line(char **line, int last_exit_status, char **envp,
		int *pipe_fd)
{
	char	*expanded_line;

	expanded_line = expand_string(*line, last_exit_status, envp, 2);
	free(*line);
	*line = expanded_line;
	if (!*line)
	{
		close_fds(pipe_fd[0], pipe_fd[1]);
		setup_signal_handlers();
		return (-1);
	}
	return (0);
}

// Helper for writing to pipe
static int	write_heredoc_line(int *pipe_fd, char *line)
{
	if (write(pipe_fd[1], line, ft_strlen(line)) == -1 || write(pipe_fd[1],
			"\n", 1) == -1)
	{
		perror("minishell: heredoc write error");
		close_fds(pipe_fd[0], pipe_fd[1]);
		free(line);
		return (-1);
	}
	return (0);
}

static int	handle_sigint_heredoc(int *pipe_fd, char *line)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		close_fds(pipe_fd[0], pipe_fd[1]);
		free(line);
		setup_signal_handlers();
		return (-2);
	}
	return (0);
}

static int	heredoc_input_loop(t_redir *redir, int last_exit_status,
		char **envp, int *pipe_fd)
{
	char	*line;
	int		should_expand;

	line = NULL;
	should_expand = !redir->is_value_quoted;
	while (1)
	{
		line = readline("> ");
		if (handle_sigint_heredoc(pipe_fd, line) == -2)
			return (-2);
		if (!line || ft_strcmp(line, redir->value) == 0)
		{
			free(line);
			break ;
		}
		if (should_expand && expand_heredoc_line(&line, last_exit_status, envp,
				pipe_fd) < 0)
			return (-1);
		if (write_heredoc_line(pipe_fd, line) < 0)
			return (-1);
		free(line);
		line = NULL;
	}
	return (0);
}

int	read_heredoc_to_pipe(t_redir *redir, int last_exit_status,
		char **envp)
{
	int	pipe_fd[2];
	int	loop_result;

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	setup_heredoc_signals();
	loop_result = heredoc_input_loop(redir, last_exit_status, envp, pipe_fd);
	setup_signal_handlers();
	close(pipe_fd[1]);
	if (loop_result < 0)
		return (loop_result);
	return (pipe_fd[0]);
}
