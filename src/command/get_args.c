/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:47:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/03 20:29:35 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_file(t_token *tkn)
{
	if ((tkn->type == REDIR_IN || tkn->type == REDIR_OUT || tkn->type == HEREDOC
			|| tkn->type == APPEND) && (tkn->next->type == WORD
			|| tkn->next->type == DOUBLE || tkn->next->type == SIMPLE))
		return (1);
	return (0);
}

static int	allocate_args_array(t_cmd *cmd, int len)
{
	cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
	if (!cmd->args)
	{
		ft_error("malloc cmd->args");
		return (0);
	}
	return (1);
}

static t_token	*get_pipe_tkn(t_token *tkn)
{
	while (tkn && tkn->type != PIPE)
		tkn = tkn->next;
	if (tkn && tkn->type == PIPE)
		tkn = tkn->next;
	if (!tkn)
		return (NULL);
	return (tkn);
}

static void	fill_command_args(t_token **tkn, t_cmd *cmd, int len)
{
	int	i;

	i = 0;
	while (*tkn && (*tkn)->type != PIPE && i < len)
	{
		if (is_file(*tkn))
			*tkn = (*tkn)->next->next;
		else
		{
			cmd->args[i] = ft_strdup((*tkn)->value);
			if (!cmd->args[i])
				return (ft_freematrix(&cmd->args), ft_error("strdup"));
			i++;
			*tkn = (*tkn)->next;
		}
	}
	cmd->args[i] = NULL;
}

void	get_args(t_token *tkn, t_cmd *cmd)
{
	int	len;

	while (cmd)
	{
		len = pipelen(tkn);
		if (!allocate_args_array(cmd, len))
			return ;
		fill_command_args(&tkn, cmd, len);
		tkn = get_pipe_tkn(tkn);
		cmd = cmd->next;
	}
}
