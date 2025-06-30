/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 01:27:48 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/30 22:22:08 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_redir	*create_redir(void)
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

static void	set_tknhead(t_token **tkn, int cmd_index)
{
	int		count;

	count = 0;
	while (*tkn && count < cmd_index)
	{
		if ((*tkn)->type == PIPE)
			count++;
		*tkn = (*tkn)->next;
	}
}

t_redir	*redir_node(t_shell *mshell, t_token *tkn, t_redir *lstrd, int cmd_index)
{
	char	*tmp;
	t_redir	*new;

	tmp = NULL;
	set_tknhead(&tkn, cmd_index);
	while (tkn && tkn->type != PIPE)
	{
		if (tkn->type == REDIR_IN || tkn->type == REDIR_OUT
			|| tkn->type == APPEND || tkn->type == HEREDOC)
		{
			new = create_redir();
			if (!new)
				return (ft_error("create rd"), NULL);
			new->type = tkn->type;
			if (tkn->next)
				tkn = tkn->next;
			tmp = ft_strdup(tkn->value);
			new->file = process_str(mshell, tmp);
			if (!new->file)
				return (ft_error("create file"), NULL);
			addlast_redir(&lstrd, new);
		}
		tkn = tkn->next;
	}
	return (lstrd);
}
