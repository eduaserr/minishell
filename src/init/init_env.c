/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:17:36 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/28 03:16:38 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	new->prev = NULL;
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

	if (!lstenv || !node)
		return ;
	if (!*lstenv)
		*lstenv = node;
	else
	{
		last = get_last_node(*lstenv);
		last->next = node;
		node->prev = last;
	}
}
