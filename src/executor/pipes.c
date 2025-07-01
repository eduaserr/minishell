/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:56:53 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/01 19:56:47 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	parent_process(t_shell *shell, int *in_fd, int fd[2])
{
	if (shell->commands->next)
	{
		close(fd[1]);
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
		*in_fd = fd[0];
	}
	else if (*in_fd != STDIN_FILENO)
		close(*in_fd);
}

static t_redir	*find_redir_by_type(t_command *command, int type)
{
	t_redir	*redir;

	if (!command)
		return (NULL);
	redir = command->rd;
	while (redir)
	{
		if (redir->type == type)
			return (redir);
		redir = redir->next;
	}
	return (NULL);
}

static void	child_process(t_shell *shell, int fd[2], int in_fd, char **env)
{
	int			heredoc_fd[2];
	t_command	*command;
	t_redir	*heredoc_redir;

	command = shell->commands;
	// If command is exit, do nothing in child (ignore exit in pipeline)
	if (command && command->args && command->args[0] && !ft_strcmp(command->args[0], "exit"))
	{
		ft_printf("exit: cannot be used in a pipeline\n");
		exit(0);
	}
	heredoc_redir = find_redir_by_type(command, HEREDOC);
	if (heredoc_redir)
	{
		if (pipe(heredoc_fd) < 0)
		{
			ft_error("child proccess");
			exit(EXIT_FAILURE);
		}
		execute_heredoc(heredoc_redir->file, heredoc_fd);
	}
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (command->next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	setup_redirection(shell, false);
	exec_cmd(shell, command->args, env);
	exit(EXIT_FAILURE);
}
void	handle_pipes(t_shell *shell, char **env)
{
	t_command	*first;
	int			pid;
	int			fd[2];
	int			in_fd;
	int			child_count;

	child_count = 0;
	first = shell->commands;
	in_fd = STDIN_FILENO;
	while (shell->commands)
	{
		if (shell->commands->next && pipe(fd) < 0)
		{
			ft_error("handle pipe");
			break ;
		}
		pid = fork();
		if (pid < 0)
		{
			ft_error("fork error");
			break ;
		}
		if (pid == 0)
		{
			child_process(shell, fd, in_fd, env);
			exit(EXIT_FAILURE);
		}
		else
		{
			child_count++;
			parent_process(shell, &in_fd, fd);
		}
		shell->commands = shell->commands->next;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	shell->commands = first;
	while (child_count-- > 0)
		wait(NULL);
}