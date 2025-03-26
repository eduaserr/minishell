/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/26 20:47:55 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_env	*split_env(t_env *new, char *env)
{
	char	**arr;
	int		i;

	i = 0;
	arr = NULL;
	arr = ft_split(env, '=');
	if (!arr || !*arr)
		return (ft_error("split"), NULL);
	ft_printf("%s\n", env);
	ft_printmatrix(arr);
	ft_printf("\n<-------->\n");
	new->key = ft_strdup(arr[0]);
	if (!new->key)
		return (NULL);
	new->value = ft_strdup(arr[1]);
	if (!new->value)
		return (NULL);
	return (ft_freematrix(arr), new);
}

static t_env	*create_env(char *env)
{
	t_env	*new;

	(void)env;
	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new = split_env(new, env);
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

static t_env	*init_env(char	**env)
{
	t_env	*new;
	int		i;

	new = NULL;
	i = 0;
	while (env && env[i])
	{
		new = create_env(env[i]);
		if (!new)
			return (NULL);
		//new->prev;
		//new->next;
		i++;
	}
	return (new);
}

t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->env = ft_init_array(envp);
	if (!mshell->env)
		return (ft_free_mshell(mshell), NULL);
	mshell->lstenv = init_env(mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(mshell), NULL);
	mshell->user_input = NULL;
	return (mshell);
}
