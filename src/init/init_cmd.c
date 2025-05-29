/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:25:52 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/29 17:22:42 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

t_command	*create_cmd(t_command *new)
{
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->cmd = NULL;
	new->redirs = NULL;
	new->next = NULL;
	return (new);
}
