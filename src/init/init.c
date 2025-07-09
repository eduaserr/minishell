/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/01 17:38:09 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_shlvl(t_env *lstenv)
{
	int		shlvl;
	char	*new_shlvl_str;

	lstenv = ft_getlstenv(lstenv, "SHLVL");
	if (!lstenv)
		return (ft_error("update shlvl"));
	shlvl = ft_atoi(lstenv->value);
	shlvl++;
	new_shlvl_str = ft_itoa(shlvl);
	if (new_shlvl_str)
	{
		free(lstenv->value);
		lstenv->value = new_shlvl_str;
	}
}

static char	**new_env(int i)
{
	char	**env;
	char	*pwd;

	pwd = NULL;
	env = NULL;
	env = (char **)ft_calloc(i, sizeof(char *));
	if (!env)
		return (NULL);
	env[0] = ft_strdup("USER=");
	if (!env[0])
		return (free(env), NULL);
	env[1] = ft_strdup("HOME=");
	if (!env[1])
		return (ft_freematrix(&env), NULL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_freematrix(&env), NULL);
	env[2] = ft_strjoin("PWD=", pwd);
	ft_free_str(&pwd);
	if (!env[2])
		return (ft_freematrix(&env), NULL);
	env[3] = ft_strdup("SHLVL=0");
	if (!env[3])
		return (ft_freematrix(&env), NULL);
	return (env);
}

static t_env	*init_env(t_env *lstenv, char **env)
{
	t_env	*new;
	int		i;

	lstenv = NULL;
	new = NULL;
	i = 0;
	while (env && env[i])
	{
		new = create_env(env[i]);
		if (!new)
			return (NULL);
		addlast_node(&lstenv, new);
		i++;
	}
	return (lstenv);
}

t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->cmd_count = 0;
	mshell->running = 1;
	mshell->exit_status = 0;
	mshell->last_exit_status = 0;
	mshell->commands = NULL;
	mshell->tkn = NULL;
	mshell->p_input = NULL;
	mshell->user_input = NULL;
	if (!envp || !*envp)
		mshell->env = new_env(5);
	else
		mshell->env = ft_init_array(envp);
	if (!mshell->env)
		return (free(mshell), NULL);
	mshell->lstenv = init_env(mshell->lstenv, mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(&mshell), NULL);
	update_shlvl(mshell->lstenv);
	return (mshell);
}

void	sync_arr_env(t_shell *shell)
{
	ft_free_env(&shell->lstenv);
	shell->lstenv = init_env(shell->lstenv, shell->env);
}
