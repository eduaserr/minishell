/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/28 14:09:04 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_printenv(t_env *lstenv)
{
	int	i;

	i = 0;
	while (lstenv)
	{
		ft_printf("node %d\n[0] - %s\n", i++, lstenv->key);
		ft_printf("[1] - %s\n", lstenv->value);
		lstenv = lstenv->next;
	}
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
	mshell->env = ft_init_array(envp);
	if (!mshell->env)
		return (ft_free_mshell(mshell), NULL);
	mshell->lstenv = init_env(mshell->lstenv, mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(mshell), NULL);
	mshell->user_input = NULL;
	return (mshell);
}
