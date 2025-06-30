/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:04:44 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/30 16:19:37 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_heredoc(char *delimiter, int heredoc_fd[2])
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
			break ;
		write(heredoc_fd[1], line, ft_strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
	dup2(heredoc_fd[0], STDIN_FILENO);
	close(heredoc_fd[0]);
}

static void	handle_file_input(t_shell *shell, char *file)
{
	int	fd_in;
	(void)shell;

	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
		return (ft_error("infile open"));
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

static void	handle_heredoc_input(t_shell *shell, char *delimiter)
{
	int	heredoc_fd[2];

	(void)shell;
	if (pipe(heredoc_fd) < 0)
		return (ft_error("heredoc input"));
	execute_heredoc(delimiter, heredoc_fd);
	dup2(heredoc_fd[0], STDIN_FILENO);
	close(heredoc_fd[0]);
}

void	redirect_stdin(t_shell *shell, bool handle_heredoc)
{
	t_command	*command;
	t_redir		*redir;

	command = shell->commands;
	if (!command || !command->rd)
		return ;
	redir = command->rd;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			handle_file_input(shell, redir->file);
		else if (redir->type == HEREDOC && handle_heredoc)
			handle_heredoc_input(shell, redir->file);
		redir = redir->next;
	}
}

void	setup_redirection(t_shell *shell, bool handle_heredoc)
{
	int			fd_out;
	t_redir		*redir;

	redirect_stdin(shell, handle_heredoc);
	if (!shell->commands || !shell->commands->rd)
		return ;
	redir = shell->commands->rd;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (redir->type == APPEND)
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0666);
			else
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if (fd_out == -1)
				return (ft_error("outfile open"));
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
			return ;
		}
		redir = redir->next;
	}
}
