/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:09:36 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/01 18:44:27 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_cmd(t_shell *shell, char **cmd_args, char **env)
{
	char	*path;
	bool	path_allocated;

	path_allocated = false;
	if (!ft_strcmp(cmd_args[0], "echo") || !ft_strcmp(cmd_args[0], "pwd")
		|| !ft_strcmp(cmd_args[0], "env"))
	{
		execute_child_builtins(cmd_args, shell);
		ft_exit_child(&shell, 0);
	}
	if (ft_strchr(cmd_args[0], '/'))
		path = cmd_args[0];
	else
	{
		path = get_cmd_paths(cmd_args[0], env);
		if (path == NULL)
			return (ft_perror("cnf", cmd_args[0]), ft_exit_child(&shell, 0));
		path_allocated = true;
	}
	if (execve(path, cmd_args, env) == -1)
	{
		if (path_allocated)
			free(path);
		return (ft_perror("cnf", cmd_args[0]), ft_exit_child(&shell, 0));
	}
}

void	execute(t_shell *shell, char **cmd_args, char **env)
{
	if (!shell->commands->next)
	{
		setup_redirection(shell, true);
		exec_cmd(shell, cmd_args, env);
	}
	else
		handle_pipes(shell, env);
}
