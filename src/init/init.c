/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/26 20:21:54 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* static char	**new_env()
{
	char	**env;

	env = ft_init_array();
	env[0] = ft_strjoin("PATH=", getcwd(NULL, 0));
	env[1] = ft_strdup("SHLVL=1");
	execve();

		HOME
		USER
		SHLVL
		PWD
		OLDPWD
		_
	
	return (env);
} */

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
	if (envp && *envp)
		mshell->env = ft_init_array(envp);
	//else
	//	mshell->env = new_env();
	if (!mshell->env)
		return (ft_free_mshell(&mshell), NULL);
	mshell->lstenv = init_env(mshell->lstenv, mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(&mshell), NULL);
	mshell->commands = NULL;
	mshell->tokens = NULL;
	mshell->p_input = NULL;
	mshell->user_input = NULL;
	mshell->running = 1;
	mshell->exit_status = 0;
	return (mshell);
}
