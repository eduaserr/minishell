/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:09:36 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 19:36:34 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*validate_and_get_path(t_shell *shell, char **cmd_args, char **env)
{
	char	*path;

	if (!cmd_args || !cmd_args[0])
		ft_exit_child(&shell, 1);
	if (ft_strchr(cmd_args[0], '/'))
	{
		if (access(cmd_args[0], F_OK) == -1)
		{
			ft_perror(shell, "nsfod", cmd_args[0]);
			ft_exit_child(&shell, 127);
		}
		if (access(cmd_args[0], X_OK) == -1)
		{
			ft_perror(shell, "Permission denied", cmd_args[0]);
			ft_exit_child(&shell, 126);
		}
		return (cmd_args[0]);
	}
	path = get_cmd_paths(cmd_args[0], env);
	if (!path)
	{
		ft_perror(shell, "cnf", cmd_args[0]);
		ft_exit_child(&shell, 127);
	}
	return (path);
}

void	exec_cmd(t_shell *shell, char **cmd_args, char **env)
{
	char	*path;

	path = validate_and_get_path(shell, cmd_args, env);
	if (execve(path, cmd_args, env) == -1)
	{
		ft_perror(shell, "execve failed", cmd_args[0]);
		ft_exit_child(&shell, 127);
	}
}

void	execute(t_shell *shell, char **cmd_args, char **env)
{
	if (!shell->commands->next)
	{
		signal(SIGINT, SIG_DFL);
		setup_redirection(shell, true);
		exec_cmd(shell, cmd_args, env);
	}
	else
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		handle_pipes(shell, env);
	}
}
