/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:09:36 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/23 14:37:32 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec_cmd(t_shell *shell, char **cmd_args, char **env)
{
	char	*path;
	bool	path_allocated;

	execute_child_builtins(cmd_args, shell);
	if (ft_strchr(cmd_args[0], '/'))
		path = cmd_args[0];
	else
	{
		path = get_cmd_paths(cmd_args[0], env);
		if (path == NULL)
		{
			errno = ENOENT;
			ft_error_exit(&shell, "Cmd not found", EXIT_CMD_NOT_FOUND);
		}
		path_allocated = true;
	}
	if (execve(path, cmd_args, env) == -1)
	{
		if (path_allocated)
			free(path);
		ft_error_exit(&shell, "Execve error", EXIT_FAILURE);
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
