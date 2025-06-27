/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:24:48 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/27 21:21:32 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	update_env_pwd(t_shell *shell, char *oldpwd_val)
{
	char	cwd[4096];
	t_env	*pwd_node;
	t_env	*oldpwd_node;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: cd: error retrieving current directory");
		return (EXIT_FAILURE);
	}
	// ✅ Actualizar OLDPWD en la lista enlazada
	if (oldpwd_val)
	{
		oldpwd_node = ft_getlstenv(shell->lstenv, "OLDPWD");
		if (oldpwd_node)
		{
			free(oldpwd_node->value);
			oldpwd_node->value = ft_strdup(oldpwd_val);
		}
		else
		{
			// Crear nuevo nodo OLDPWD
			oldpwd_node = create_env("OLDPWD=");
			if (oldpwd_node)
			{
				free(oldpwd_node->value);
				oldpwd_node->value = ft_strdup(oldpwd_val);
				addlast_node(&shell->lstenv, oldpwd_node);
			}
		}
	}
    // ✅ Actualizar PWD en la lista enlazada
    pwd_node = ft_getlstenv(shell->lstenv, "PWD");
    if (pwd_node)
    {
        free(pwd_node->value);
        pwd_node->value = ft_strdup(cwd);
    }
    else
    {
        // Crear nuevo nodo PWD
        pwd_node = create_env("PWD=");
        if (pwd_node)
        {
            free(pwd_node->value);
            pwd_node->value = ft_strdup(cwd);
            addlast_node(&shell->lstenv, pwd_node);
        }
    }
    
    // ✅ Sincronizar con el array env
    sync_env_array(shell);
    
    return (EXIT_SUCCESS);
}

static int	cd_to_oldpwd(t_shell *shell)
{
    char	*path_to_oldpwd;
    char	*current_pwd;

    current_pwd = ft_getenv(shell->lstenv, "PWD");  // ✅ Sintaxis correcta
    if (!current_pwd)
        return (EXIT_FAILURE);
    
    path_to_oldpwd = ft_getenv(shell->lstenv, "OLDPWD");  // ✅ Sintaxis correcta
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

    oldpwd_val = ft_getenv(shell->lstenv, "PWD");  // ✅ Sintaxis correcta
    if (!oldpwd_val)
        return (EXIT_FAILURE);
    
    path_to_home = ft_getenv(shell->lstenv, "HOME");  // ✅ Sintaxis correcta
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
