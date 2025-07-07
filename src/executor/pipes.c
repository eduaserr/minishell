/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 11:56:53 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 18:42:18 by aamoros-         ###   ########.fr       */
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

static void	child_process(t_shell *shell, int fd[2], int in_fd, char **env)
{
	t_cmd	*command;

	signal_function();
	command = shell->commands;
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
	ft_exit_child(&shell, EXIT_FAILURE);
}

int	spawn_child(t_shell *shell, t_pipe_ctx *ctx, char **env)
{
	int	pid;

	pid = fork();
	if (pid < 0)
	{
		ft_error("fork error");
		return (-1);
	}
	if (pid == 0)
		child_process(shell, ctx->fd, ctx->in_fd, env);
	else
	{
		(*ctx->child_count)++;
		parent_process(shell, &ctx->in_fd, ctx->fd);
	}
	return (0);
}

void	wait_for_children(int count, t_shell *shell)
{
	int	status;

	while (count-- > 0)
		wait(&status);
	ft_exit_child(&shell, WEXITSTATUS(status));
}

void	handle_pipes(t_shell *shell, char **env)
{
	t_cmd		*first;
	t_pipe_ctx	ctx;
	int			child_count;

	first = shell->commands;
	ctx.in_fd = STDIN_FILENO;
	child_count = 0;
	ctx.child_count = &child_count;
	while (shell->commands)
	{
		if (setup_pipe_if_needed(shell, ctx.fd) < 0)
			break ;
		if (spawn_child(shell, &ctx, env) < 0)
			break ;
		shell->commands = shell->commands->next;
	}
	shell->commands = first;
	wait_for_children(child_count, shell);
}
