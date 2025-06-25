/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:25:52 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/25 22:58:10 by eduaserr         ###   ########.fr       */
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

t_command	*create_cmd(t_command *new)
{
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->cmd = NULL;
	new->args = NULL;
	new->rd = NULL;
	new->next = NULL;
	return (new);
}

static t_command	*get_lastcmd_node(t_command *node)
{
	while (node->next)
		node = node->next;
	return (node);
}

void	addlastcmd_node(t_command **lstcmd, t_command *node)
{
	t_command	*last;

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
