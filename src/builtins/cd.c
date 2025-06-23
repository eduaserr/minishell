/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:24:48 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/23 16:16:03 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	update_env_pwd(t_shell *shell, char *oldpwd_val)
{
	char	cwd[4096];
	char	*pwd_env;
	char	*oldpwd_env;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (EXIT_FAILURE);
	}
	if (oldpwd_val)
	{
		oldpwd_env = ft_strjoin("OLDPWD=", oldpwd_val);
		if (!oldpwd_env)
			return (EXIT_FAILURE);
		set_env(oldpwd_env, &shell->env);
		free(oldpwd_env);
	}
	pwd_env = ft_strjoin("PWD=", cwd);
	if (!pwd_env)
		return (EXIT_FAILURE);
	set_env(pwd_env, &shell->env);
	free(pwd_env);
	return (EXIT_SUCCESS);
}

static int	cd_to_oldpwd(t_shell *shell)
{
	char	*path_to_oldpwd;
	char	*current_pwd;

	current_pwd = get_env("PWD", shell->env);
	path_to_oldpwd = get_env("OLDPWD", shell->env);
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

	oldpwd_val = get_env("PWD", shell->env);
	path_to_home = get_env("HOME", shell->env);
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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	builtin_cd(t_shell *shell, char **cmd)
{
	char	*oldpwd_val;

	if (!cmd[1] || ft_strcmp(cmd[1], "~") == 0)
		return (cd_to_home(shell));
	if (ft_strcmp(cmd[1], "-") == 0)
		return (cd_to_oldpwd(shell));
	oldpwd_val = get_env("PWD", shell->env);
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
