/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:04:50 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/09 15:10:19 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	execute_parent_builtin(char **cmd_args, t_shell *shell)
{
	if (!cmd_args[0])
		return (false);
	if (ft_strcmp(cmd_args[0], "cd") == 0)
	{
		shell->last_exit_status = builtin_cd(shell, cmd_args);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "exit") == 0 && shell->cmd_count < 1)
	{
		builtin_exit(cmd_args);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "unset") == 0)
	{
		shell->last_exit_status = builtin_unset(shell, cmd_args);
		sync_arr_env(shell);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "export") == 0)
	{
		shell->last_exit_status = builtin_export(shell, cmd_args);
		sync_arr_env(shell);
		return (true);
	}
	return (false);
}

int	execute_child_builtins(char **cmd_args, t_shell *shell)
{
	t_cmd	temp_cmd;

	if (!cmd_args || !cmd_args[0])
		return (-1);
	temp_cmd.cmd = cmd_args[0];
	temp_cmd.args = cmd_args;
	temp_cmd.rd = NULL;
	temp_cmd.next = NULL;
	if (!ft_strncmp(cmd_args[0], "echo", 5))
	{
		shell->last_exit_status = builtin_echo(&temp_cmd);
		return (1);
	}
	if (!ft_strncmp(cmd_args[0], "pwd", 4))
	{
		shell->last_exit_status = builtin_pwd();
		return (1);
	}
	if (!ft_strncmp(cmd_args[0], "env", 4))
		return (builtin_env(shell->env), 1);
	if (!ft_strncmp(cmd_args[0], "exit", 5))
		exit(1);
	return (execute_parent_builtin(cmd_args, shell));
}
