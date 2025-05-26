/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:18:07 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/29 15:36:14 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Echo builtin
static int	parse_echo_flags(char **args, int *suppress_newline)
{
	int		i;
	char	*p;

	i = 1;
	*suppress_newline = 0;
	while (args && args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		p = args[i] + 1;
		while (*p == 'n')
			p++;
		if (*p == '\0')
		{
			*suppress_newline = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}

static int	builtin_echo(t_command *cmd)
{
	int		i;
	int		suppress_newline;
	char	**args;

	args = cmd->argv;
	i = parse_echo_flags(args, &suppress_newline);
	while (args && args[i])
	{
		write(STDOUT_FD, args[i], ft_strlen(args[i]));
		i++;
		if (args[i])
			write(STDOUT_FD, " ", 1);
	}
	if (!suppress_newline)
		write(STDOUT_FD, "\n", 1);
	return (0);
}

// Main export function
int	builtin_export(t_command *cmd, char ***envp_ptr)
{
	if (!cmd->argv || !cmd->argv[1])
		return (print_exported_vars(*envp_ptr));
	return (process_export_args(cmd->argv, envp_ptr));
}

int	is_builtin(char *command)
{
	if (!command)
		return (0);
	if (ft_strcmp(command, "echo") == 0 || ft_strcmp(command, "cd") == 0
		|| ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "unset") == 0 || ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "exit") == 0)
	{
		return (1);
	}
	return (0);
}

int	execute_builtin(t_command *cmd, char ***envp_ptr, int is_child)
{
	char	*command_name;

	if (cmd->argv && cmd->argv[0])
		command_name = cmd->argv[0];
	else
		command_name = NULL;
	if (!command_name)
		return (EXIT_FAILURE);
	if (ft_strcmp(command_name, "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(command_name, "pwd") == 0)
		return (builtin_pwd(cmd));
	if (ft_strcmp(command_name, "env") == 0)
		return (builtin_env(cmd, *envp_ptr));
	if (ft_strcmp(command_name, "cd") == 0)
		return (builtin_cd(cmd, envp_ptr));
	if (ft_strcmp(command_name, "export") == 0)
		return (builtin_export(cmd, envp_ptr));
	if (ft_strcmp(command_name, "unset") == 0)
		return (builtin_unset(cmd, envp_ptr));
	if (ft_strcmp(command_name, "exit") == 0)
		return (builtin_exit(cmd, is_child));
	return (EXIT_FAILURE);
}
