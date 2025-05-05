/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:25:52 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/05 18:07:04 by eduaserr         ###   ########.fr       */
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
