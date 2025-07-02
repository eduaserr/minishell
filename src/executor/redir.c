/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:04:44 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/02 18:07:26 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_heredoc(char *delimiter, int heredoc_fd[2])
{
	char	*line;

	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd[1], line, ft_strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
}

static void	handle_file_input(t_shell *shell, char *file)
{
	int	fd_in;

	(void)shell;
	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
		return (ft_perror("No such file or directory", file),
			ft_exit_child(&shell, 1));
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

static int	handle_heredoc_input(t_shell *shell, char *delimiter)
{
	int	heredoc_fd[2];

	(void)shell;
	if (pipe(heredoc_fd) < 0)
		return (ft_error("heredoc input"), ft_exit_child(&shell, 1), -1);
	execute_heredoc(delimiter, heredoc_fd);
	return (heredoc_fd[0]);
}

void	redirect_stdin(t_shell *shell, bool handle_heredoc)
{
	t_command	*command;
	t_redir		*redir;
	t_redir		*last_stdin_redir;
	int			heredoc_fd;

	command = shell->commands;
	if (!command || !command->rd)
		return ;
	last_stdin_redir = NULL;
	redir = command->rd;
	while (redir)
	{
		if (redir->type == REDIR_IN || redir->type == HEREDOC)
			last_stdin_redir = redir;
		redir = redir->next;
	}
	if (handle_heredoc)
	{
		redir = command->rd;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				heredoc_fd = handle_heredoc_input(shell, redir->file);
				if (redir == last_stdin_redir)
				{
					dup2(heredoc_fd, STDIN_FILENO);
					close(heredoc_fd);
				}
				else
				{
					close(heredoc_fd);
				}
			}
			redir = redir->next;
		}
	}
	if (last_stdin_redir && last_stdin_redir->type == REDIR_IN)
		handle_file_input(shell, last_stdin_redir->file);
}

void	setup_redirection(t_shell *shell, bool handle_heredoc)
{
	int fd_out;
	t_redir *redir;
	t_redir *last_output_redir;

	redirect_stdin(shell, handle_heredoc);
	if (!shell->commands || !shell->commands->rd)
		return ;
	redir = shell->commands->rd;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (redir->type == APPEND)
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

			if (fd_out == -1)
				return (ft_perror("minishell", redir->file),
					ft_exit_child(&shell, EXIT_FAILURE));
			close(fd_out);
		}
		redir = redir->next;
	}
	last_output_redir = NULL;
	redir = shell->commands->rd;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
			last_output_redir = redir;
		redir = redir->next;
	}
	if (last_output_redir)
	{
		if (last_output_redir->type == APPEND)
			fd_out = open(last_output_redir->file, O_WRONLY | O_APPEND);
		else
			fd_out = open(last_output_redir->file, O_WRONLY);

		if (fd_out == -1)
			return (ft_perror("minishell", last_output_redir->file),
				ft_exit_child(&shell, EXIT_FAILURE));
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}