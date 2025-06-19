/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:27:48 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/19 03:36:52 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_redir	*create_redir()
{
	t_redir	*new;

	new = (t_redir *)malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->file = NULL;
	new->next = NULL;
	return (new);
}

static t_redir	*get_last_redir(t_redir *node)
{
	while (node->next)
		node = node->next;
	return (node);
}

void	addlast_redir(t_redir **lstrd, t_redir *node)
{
	t_redir	*last;

	last = NULL;
	if (!lstrd || !node)
		return ;
	if (!*lstrd)
		*lstrd = node;
	else
	{
		last = get_last_redir(*lstrd);
		last->next = node;
	}
}

t_redir	*redir_node(t_token *tkn, t_redir *lstrd)
{
	t_redir	*new;

	int	i = 0;
	while (tkn && tkn->type != PIPE)
	{
		ft_printf("entra if\n");
		new = create_redir();
		if (!new)
			return (ft_error("create rd"), NULL);
		new->type = tkn->type;
		ft_printf("nodo [%i]\nvalue -> %s\n", i++, tkn->value);
		addlast_redir(&lstrd, new);
		ft_printf("AÑADE NODO\n");
		tkn = tkn->next;
	}
	return (lstrd);
}
