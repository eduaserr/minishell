/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:04:50 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/17 19:19:15 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	execute_parent_builtin(char **cmd_args, t_shell *shell)
{
	if (!ft_strncmp(cmd_args[0], "cd", 5))
	{
		shell->last_exit_status = builtin_cd(shell, cmd_args);
		ft_free_cmd(shell->commands);
		return (true);
	}
	if (!ft_strncmp(cmd_args[0], "exit", 5))
	{
		builtin_exit(cmd_args, shell);
		ft_free_cmd(shell->commands);
		return (true);
	}
	if (!ft_strncmp(cmd_args[0], "unset", 6))
	{
		shell->last_exit_status = builtin_unset(cmd_args, shell);
		ft_free_cmd(shell->commands);
		return (true);
	}
	if (!ft_strncmp(cmd_args[0], "export", 7))
	{
		shell->last_exit_status = builtin_export(cmd_args, shell);
		ft_free_cmd(shell->commands);
		return (true);
	}
	return (false);
}

void	execute_child_builtins(char **cmd_args, t_shell *shell)
{
	if (!ft_strncmp(cmd_args[0], "echo", 5))
		ft_echo(cmd_args, shell);
	if (!ft_strncmp(cmd_args[0], "pwd", 4))
		ft_pwd(shell);
	if (!ft_strncmp(cmd_args[0], "env", 4))
		ft_env(shell->env, shell);
}
