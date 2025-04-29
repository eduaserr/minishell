/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:51:31 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/29 14:52:01 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1. Function to determine target path
static int	get_target_path(t_command *cmd, char ***envp_ptr,
		char **target_path)
{
	if (!cmd->argv || !cmd->argv[1])
	{
		*target_path = get_env_value("HOME", *envp_ptr);
		if (!*target_path || **target_path == '\0')
		{
			print_error("cd", "HOME not set");
			return (1);
		}
	}
	else if (cmd->argv[2])
	{
		print_error("cd", "too many arguments");
		return (1);
	}
	else
		*target_path = cmd->argv[1];
	return (0);
}

// 2. Function to get old PWD value
static char	*get_old_pwd(char ***envp_ptr)
{
	char	*old_pwd_val;
	char	*pwd_from_env;

	old_pwd_val = getcwd(NULL, 0);
	if (!old_pwd_val)
	{
		pwd_from_env = get_env_value("PWD", *envp_ptr);
		if (pwd_from_env)
		{
			old_pwd_val = ft_strdup(pwd_from_env);
			if (!old_pwd_val)
				perror("minishell: cd: strdup failed");
		}
	}
	return (old_pwd_val);
}

// 3. Function to update PWD environment variables
static int	update_pwd_vars(char *old_pwd, char *new_pwd, char ***envp_ptr)
{
	int	ret;

	ret = 0;
	if (old_pwd)
	{
		if (set_env_var("OLDPWD", old_pwd, envp_ptr) != 0)
		{
			print_error("cd", "failed to set OLDPWD (malloc error)");
			ret = 1;
		}
	}
	else
		unset_env_var("OLDPWD", envp_ptr);
	if (new_pwd)
	{
		if (set_env_var("PWD", new_pwd, envp_ptr) != 0)
		{
			print_error("cd", "failed to set PWD (malloc error)");
			ret = 1;
		}
	}
	return (ret);
}

// Main cd function using the 3 helper functions
static int	builtin_cd(t_command *cmd, char ***envp_ptr)
{
	char	*target_path;
	char	*old_pwd_val;
	char	*new_pwd_val;
	int		ret_env;

	if (get_target_path(cmd, envp_ptr, &target_path))
		return (1);
	old_pwd_val = get_old_pwd(envp_ptr);
	if (chdir(target_path) == -1)
	{
		print_error_detail("cd", target_path, strerror(errno));
		free(old_pwd_val);
		return (1);
	}
	new_pwd_val = getcwd(NULL, 0);
	if (!new_pwd_val)
		perror("minishell: cd: getcwd failed after chdir");
	ret_env = update_pwd_vars(old_pwd_val, new_pwd_val, envp_ptr);
	free(old_pwd_val);
	free(new_pwd_val);
	return (ret_env);
}
