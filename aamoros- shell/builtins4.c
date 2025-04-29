/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:34:10 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/29 15:35:15 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_command *cmd, char ***envp_ptr)
{
	int		i;
	int		exit_status;
	char	*arg;

	i = 0;
	exit_status = 0;
	if (!cmd->argv || !cmd->argv[1])
		return (0);
	while (cmd->argv[++i])
	{
		arg = cmd->argv[i];
		if (!is_valid_var_name(arg) || ft_strchr(arg, '='))
		{
			print_error_detail("unset", arg, "not a valid identifier");
			exit_status = 1;
		}
		else
		{
			if (remove_env_var(arg, envp_ptr) != 0)
				return (print_error("unset", "memory allocation failed"), 1);
		}
	}
	return (exit_status);
}

int	builtin_env(t_command *cmd, char **envp)
{
	int	i;

	i = 0;
	(void)cmd;
	if (cmd->argv && cmd->argv[1])
	{
		print_error("env", "too many arguments");
		return (1);
	}
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			write(STDOUT_FD, envp[i], ft_strlen(envp[i]));
			write(STDOUT_FD, "\n", 1);
		}
		i++;
	}
	return (0);
}

int	builtin_exit(t_command *cmd, int is_child)
{
	long long	status_ll;
	int			error_flag;

	error_flag = 0;
	if (!is_child)
		write(STDERR_FD, "exit\n", 5);
	if (!cmd->argv || !cmd->argv[1])
		return (EXIT_REQUEST_SIG_OFFSET + 0);
	status_ll = ft_llatoi_strict(cmd->argv[1], &error_flag);
	if (error_flag)
	{
		print_error_detail("exit", cmd->argv[1], "numeric argument required");
		return (EXIT_BUILTIN_MISUSE);
	}
	if (cmd->argv[2])
	{
		print_error("exit", "too many arguments");
		return (EXIT_FAILURE);
	}
	return (EXIT_REQUEST_SIG_OFFSET + (unsigned char)status_ll);
}

int	builtin_pwd(t_command *cmd)
{
	char	cwd_buffer[PATH_MAX];

	(void)cmd;
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
	{
		write(STDOUT_FD, cwd_buffer, ft_strlen(cwd_buffer));
		write(STDOUT_FD, "\n", 1);
		return (0);
	}
	else
	{
		print_error_detail("pwd", "getcwd", strerror(errno));
		return (1);
	}
}
