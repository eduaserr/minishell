/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 18:04:50 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/30 15:40:19 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	execute_parent_builtin(char **cmd_args, t_shell *shell)
{
	(void)shell;
	if (ft_strcmp(cmd_args[0], "cd") == 0)
	{
		shell->last_exit_status = builtin_cd(shell, cmd_args);
		//ft_free_cmd(&shell->commands);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "exit") == 0)
	{
		builtin_exit(cmd_args);
		//ft_free_cmd(&shell->commands);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "unset") == 0)
	{
		shell->last_exit_status = builtin_unset(shell, cmd_args);
		//ft_free_cmd(&shell->commands);
		return (true);
	}
	if (ft_strcmp(cmd_args[0], "export") == 0)
	{
		shell->last_exit_status = builtin_export(shell, cmd_args);
		//ft_free_cmd(&shell->commands);
		return (true);
	}
	return (false);
}

void	execute_child_builtins(char **cmd_args, t_shell *shell)
{
	t_command	temp_cmd;

	if (!cmd_args || !cmd_args[0])
		return ;
	temp_cmd.cmd = cmd_args[0];
	temp_cmd.args = cmd_args;
	temp_cmd.rd = NULL;
	temp_cmd.next = NULL;
	if (!ft_strncmp(cmd_args[0], "echo", 5))
		builtin_echo(&temp_cmd);
	if (!ft_strncmp(cmd_args[0], "pwd", 4))
		builtin_pwd();
	if (!ft_strncmp(cmd_args[0], "env", 4))
		builtin_env(shell, shell->env);
}
