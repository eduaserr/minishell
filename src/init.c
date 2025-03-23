/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/21 13:14:37 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*char	**split_env(t_shell *mshell, char **env)
{
	char	**arr;
	int		i;

	i = 0;
	arr = NULL;
	while (env[i])
	{
		arr = ft_split(env[i], '=');
		ft_printf("[%d] - %s\n", i, env[i]);
		ft_printmap(arr);
		ft_printf("\n<-------->\n");
		if (ft_strncmp(env[0], "USER=", 5))
		{

		}
		i++;
	}
	return (NULL);
}*/

static	t_env	*create_env(char **env)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	//env_values()
		//key == env[0];
	new->key = NULL;
	new->value = NULL;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

static	t_env	*init_env(char	**env)
{
	t_env	*new;

	new = NULL;
	new = create_env(env);
	if (!new)
		return (NULL);
	return (new);
}

t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->lstenv = init_env(mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(mshell), NULL);
	mshell->env = ft_init_array(envp);
	if (!mshell->env)
		return (ft_free_mshell(mshell), NULL);
	mshell->user_input = NULL;
	return (mshell);
}
