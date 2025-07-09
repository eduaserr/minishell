/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:25:52 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/03 20:32:12 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
//sin usar
int	ft_nodelen(t_token *tkn)
{
	int		i;

	i = 0;
	while (tkn)
	{
		i++;
		tkn = tkn->next;
	}
	return (i);
}

t_cmd	*create_cmd(t_cmd *new)
{
	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->args = NULL;
	new->rd = NULL;
	new->next = NULL;
	return (new);
}

static t_cmd	*get_lastcmd_node(t_cmd *node)
{
	while (node->next)
		node = node->next;
	return (node);
}

void	addlastcmd_node(t_cmd **lstcmd, t_cmd *node)
{
	t_cmd	*last;

	last = NULL;
	if (!lstcmd || !node)
		return ;
	if (!*lstcmd)
		*lstcmd = node;
	else
	{
		last = get_lastcmd_node(*lstcmd);
		last->next = node;
	}
}
