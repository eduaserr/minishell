/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:15:57 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/03 19:25:56 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_exported_vars(char **envp)
{
	int		i;
	char	*eq_pos;

	i = 0;
	while (envp && envp[i])
	{
		eq_pos = strchr(envp[i], '=');
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		if (eq_pos)
		{
			write(STDOUT_FILENO, envp[i], (eq_pos - envp[i]) + 1);
			write(STDOUT_FILENO, "\"", 1);
			write(STDOUT_FILENO, eq_pos + 1, strlen(eq_pos + 1));
			write(STDOUT_FILENO, "\"", 1);
		}
		else
			ft_putstr_fd(envp[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

int	builtin_unset(t_shell *shell, char **cmd)
{
	int	i;

	i = 0;
	while (cmd[++i])
		unset_env(cmd[i], shell->env);
	return (0);
}

int	builtin_export(t_shell *shell, char **cmd)
{
	int	i;

	if (!cmd[1])
	{
		print_exported_vars(shell->env);
		return (0);
	}
	i = 1;
	while (cmd[i])
	{
		if (set_env(cmd[i], &shell->env) == -1)
			return (1);
		i++;
	}
	return (0);
}

void	builtin_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_putstr_fd(envp[i], STDOUT_FILENO);
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
