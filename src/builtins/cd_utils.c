/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:15:11 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/03 18:15:48 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	update_oldpwd(t_shell *shell, char *oldpwd_val)
{
	t_env	*oldpwd_node;

	if (!oldpwd_val)
		return (EXIT_SUCCESS);
	oldpwd_node = ft_getlstenv(shell->lstenv, "OLDPWD");
	if (oldpwd_node)
	{
		free(oldpwd_node->value);
		oldpwd_node->value = ft_strdup(oldpwd_val);
		if (!oldpwd_node->value)
			return (EXIT_FAILURE);
	}
	else
	{
		oldpwd_node = create_env("OLDPWD=");
		if (!oldpwd_node)
			return (EXIT_FAILURE);
		free(oldpwd_node->value);
		oldpwd_node->value = ft_strdup(oldpwd_val);
		if (!oldpwd_node->value)
			return (free(oldpwd_node->key), free(oldpwd_node), EXIT_FAILURE);
		addlast_node(&shell->lstenv, oldpwd_node);
	}
	return (EXIT_SUCCESS);
}

static int	update_pwd(t_shell *shell, char *cwd)
{
	t_env	*pwd_node;

	pwd_node = ft_getlstenv(shell->lstenv, "PWD");
	if (pwd_node)
	{
		free(pwd_node->value);
		pwd_node->value = ft_strdup(cwd);
		if (!pwd_node->value)
			return (EXIT_FAILURE);
	}
	else
	{
		pwd_node = create_env("PWD=");
		if (!pwd_node)
			return (EXIT_FAILURE);
		free(pwd_node->value);
		pwd_node->value = ft_strdup(cwd);
		if (!pwd_node->value)
			return (free(pwd_node->key), free(pwd_node), EXIT_FAILURE);
		addlast_node(&shell->lstenv, pwd_node);
	}
	return (EXIT_SUCCESS);
}

int	update_env_pwd(t_shell *shell, char *oldpwd_val)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (EXIT_FAILURE);
	}
	if (update_oldpwd(shell, oldpwd_val) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (update_pwd(shell, cwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	sync_env_array(shell);
	return (EXIT_SUCCESS);
}
