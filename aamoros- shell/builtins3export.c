/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:15:47 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/29 15:25:21 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1. Print all exported variables (no arguments case)
int	print_exported_vars(char **envp)
{
	int		j;
	char	*eq_pos;

	j = 0;
	while (envp && envp[j])
	{
		eq_pos = ft_strchr(envp[j], '=');
		write(STDOUT_FD, "declare -x ", 11);
		if (eq_pos)
		{
			write(STDOUT_FD, envp[j], (eq_pos - envp[j]) + 1);
			write(STDOUT_FD, "\"", 1);
			write(STDOUT_FD, eq_pos + 1, ft_strlen(eq_pos + 1));
			write(STDOUT_FD, "\"", 1);
		}
		else
		{
			write(STDOUT_FD, envp[j], ft_strlen(envp[j]));
		}
		write(STDOUT_FD, "\n", 1);
		j++;
	}
	return (0);
}

// 2. Process a single export argument
int	handle_export_assignment(char *arg, char *eq_pos, char ***envp_ptr)
{
	char	*name;
	char	*value;
	int		ret;

	name = ft_substr(arg, 0, eq_pos - arg);
	value = ft_strdup(eq_pos + 1);
	ret = 0;
	if (!name || !value)
	{
		print_error("export", "memory allocation failed");
		free(name);
		free(value);
		return (1);
	}
	if (eq_pos == arg || !is_valid_var_name(name))
	{
		print_error_detail("export", arg, "not a valid identifier");
		ret = 1;
	}
	else if (set_export_var(name, value, envp_ptr) != 0)
		ret = 1;
	free(name);
	free(value);
	return (ret);
}

int	process_single_export_arg(char *arg, char ***envp_ptr)
{
	char	*eq_pos;
	int		ret;

	eq_pos = ft_strchr(arg, '=');
	ret = 0;
	if (eq_pos)
		ret = handle_export_assignment(arg, eq_pos, envp_ptr);
	else if (!is_valid_var_name(arg))
	{
		print_error_detail("export", arg, "not a valid identifier");
		ret = 1;
	}
	return (ret);
}


// 3. Set environment variable with error handling
int	set_export_var(char *name, char *value, char ***envp_ptr)
{
	if (set_env_var(name, value, envp_ptr) != 0)
	{
		print_error("export", "memory allocation failed");
		return (1);
	}
	return (0);
}

// 4. Process all export arguments
int	process_export_args(char **args, char ***envp_ptr)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	while (args[i])
	{
		exit_status |= process_single_export_arg(args[i], envp_ptr);
		i++;
	}
	return (exit_status);
}
