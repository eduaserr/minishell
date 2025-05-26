/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executorSingleCMD.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:39:02 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/19 17:51:08 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_environment(t_command *cmd, int pipe_in_fd,
		int pipe_out_fd)
{
	reset_signal_handlers();
	if (!apply_redirections(cmd, pipe_in_fd, pipe_out_fd))
		exit(EXIT_FAILURE);
	if (pipe_in_fd != STDIN_FILENO)
		close(pipe_in_fd);
	if (pipe_out_fd != STDOUT_FILENO)
		close(pipe_out_fd);
}

void	run_external_command(t_command *cmd, char **envp)
{
	char	*command_path;

	command_path = find_command_path(cmd->argv[0]);
	if (!command_path)
	{
		print_error_detail(cmd->argv[0], NULL, "command not found");
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(command_path, cmd->argv, envp);
	print_error_detail(cmd->argv[0], NULL, strerror(errno));
	free(command_path);
	exit(EXIT_FAILURE);
}

static void	execute_builtin_or_external(t_command *cmd, char ***envp_ptr)
{
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, envp_ptr, 1));
	else
		run_external_command(cmd, *envp_ptr);
}

void	handle_child_process(t_command *cmd, char ***envp_ptr,
		int pipe_in_fd, int pipe_out_fd)
{
	setup_child_environment(cmd, pipe_in_fd, pipe_out_fd);
	if (cmd->argv && cmd->argv[0])
		execute_builtin_or_external(cmd, envp_ptr);
	else
		exit(EXIT_SUCCESS);
}

pid_t	execute_single_command(t_command *cmd, char ***envp_ptr, int pipe_in_fd,
		int pipe_out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork failed");
		return (-1);
	}
	else if (pid == 0)
		handle_child_process(cmd, envp_ptr, pipe_in_fd, pipe_out_fd);
	return (pid);
}
