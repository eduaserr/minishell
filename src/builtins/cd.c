/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:24:48 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/04 02:59:22 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	cd_to_oldpwd(t_shell *shell)
{
	char	*path_to_oldpwd;
	char	*current_pwd;

	current_pwd = ft_getenv(shell->lstenv, "PWD");
	if (!current_pwd)
		return (EXIT_FAILURE);
	path_to_oldpwd = ft_getenv(shell->lstenv, "OLDPWD");
	if (!path_to_oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		free(current_pwd);
		return (EXIT_FAILURE);
	}
	if (chdir(path_to_oldpwd) == -1)
	{
		perror("minishell: cd");
		free(current_pwd);
		free(path_to_oldpwd);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(path_to_oldpwd, 1);
	update_env_pwd(shell, current_pwd);
	free(current_pwd);
	free(path_to_oldpwd);
	return (EXIT_SUCCESS);
}

static int	cd_to_home(t_shell *shell)
{
	char	*path_to_home;
	char	*oldpwd_val;

	oldpwd_val = ft_getenv(shell->lstenv, "PWD");
	if (!oldpwd_val)
		return (EXIT_FAILURE);
	path_to_home = ft_getenv(shell->lstenv, "HOME");
	if (!path_to_home)
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		free(oldpwd_val);
		return (EXIT_FAILURE);
	}
	if (chdir(path_to_home) == -1)
	{
		perror("minishell: cd");
		free(path_to_home);
		free(oldpwd_val);
		return (EXIT_FAILURE);
	}
	update_env_pwd(shell, oldpwd_val);
	free(path_to_home);
	free(oldpwd_val);
	return (EXIT_SUCCESS);
}

static int	count_args(char **cmd)
{
	int	count;

	count = 0;
	while (cmd[count])
		count++;
	return (count);
}

int	builtin_cd(t_shell *shell, char **cmd)
{
	char	*oldpwd_val;
	int		arg_count;

	arg_count = count_args(cmd);
	if (arg_count > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", 1);
		return (EXIT_FAILURE);
	}
	if (!cmd[1] || ft_strcmp(cmd[1], "~") == 0)
		return (cd_to_home(shell));
	if (ft_strcmp(cmd[1], "-") == 0)
		return (cd_to_oldpwd(shell));
	oldpwd_val = ft_getenv(shell->lstenv, "PWD");
	if (chdir(cmd[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(cmd[1]);
		free(oldpwd_val);
		return (EXIT_FAILURE);
	}
	update_env_pwd(shell, oldpwd_val);
	free(oldpwd_val);
	return (EXIT_SUCCESS);
}
