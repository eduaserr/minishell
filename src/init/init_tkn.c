/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tkn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:04:02 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/25 22:54:21 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*get_token(t_token *new, t_token_type tkn, char *value, int *i)
{
	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	*i += ft_strlen(value);
	new->type = tkn;
	new->value = ft_strdup(value);
	if (!new->value)
		return (free(new), ft_error("dup value"), NULL);
	new->next = NULL;
	return (new);
}

static t_token	*get_lasttkn_node(t_token *node)
{
	while (node->next)
		node = node->next;
	return (node);
}

void	addlast_tknnode(t_token **token_list, t_token *node)
{
	t_token	*last;

	last = NULL;
	if (!token_list || !node)
		return ;
	if (!*token_list)
		*token_list = node;
	else
	{
		last = get_lasttkn_node(*token_list);
		last->next = node;
	}
}
