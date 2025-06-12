/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:17:36 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:07:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_env	*split_env(t_env *new, char *env)
{
	char	**arr;

	arr = NULL;
	arr = ft_split(env, '=');
	if (!arr || !arr[0])
		return (ft_error("split"), NULL);
	new->key = ft_strdup(arr[0]);
	if (!new->key)
		return (ft_freematrix(&arr), NULL);
	if (!arr[1])
		new->value = ft_strdup("");
	else
		new->value = ft_strdup(arr[1]);
	if (!new->value)
		return (ft_freematrix(&arr), NULL);
	return (ft_freematrix(&arr), new);
}

t_env	*create_env(char *env)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = NULL;
	new->value = NULL;
	new = split_env(new, env);
	if (!new)
		return (NULL);
	new->next = NULL;
	return (new);
}

static t_env	*get_last_node(t_env *node)
{
	while (node->next)
		node = node->next;
	return (node);
}

void	addlast_node(t_env **lstenv, t_env *node)
{
	t_env	*last;

	last = NULL;
	if (!lstenv || !node)
		return ;
	if (!*lstenv)
		*lstenv = node;
	else
	{
		last = get_last_node(*lstenv);
		last->next = node;
	}
}
