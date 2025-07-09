/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:56:53 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/09 14:46:44 by eduaserr         ###   ########.fr       */
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

void	wait_for_children(int count, t_shell *shell)
{
	int	status;

	while (count-- > 0)
		wait(&status);
	ft_exit_child(&shell, WEXITSTATUS(status));
}

static void	exec_pipe_cmd(t_shell *shell, char **env, int *in_fd, int fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal_function();
		if (*in_fd != STDIN_FILENO)
		{
			dup2(*in_fd, STDIN_FILENO);
			close(*in_fd);
		}
		else
			setup_redirection(shell, true);
		if (shell->commands->next)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
		}
		if (execute_child_builtins(shell->commands->args, shell) == 0)
			exec_cmd(shell, shell->commands->args, env);
		ft_exit_child(&shell, EXIT_FAILURE);
	}
	else
		parent_process(shell, in_fd, fd);
}

void	handle_pipes(t_shell *shell, char **env)
{
	int		in_fd;
	int		fd[2];
	t_cmd	*current_cmd;

	in_fd = STDIN_FILENO;
	current_cmd = shell->commands;
	while (current_cmd)
	{
		shell->commands = current_cmd;
		if (current_cmd->next && pipe(fd) == -1)
			return (ft_error("pipe failed"));
		exec_pipe_cmd(shell, env, &in_fd, fd);
		current_cmd = current_cmd->next;
	}
	wait_for_children(shell->cmd_count, shell);
}
