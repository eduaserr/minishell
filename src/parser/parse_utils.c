/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:40:15 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 18:40:25 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_pipes_err(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '|' && i == 0)
			return (SYNTAX_ERROR_STATUS);
		if (str[i] == '|' && str[i + 1] == '|')
			return (SYNTAX_ERROR_STATUS);
		i++;
	}
	if (i > 0 && str[i - 1] == '|')
		return (SYNTAX_ERROR_STATUS);
	return (0);
}

int	handle_rd_err(t_shell *shell, t_token *tkn)
{
	while (tkn)
	{
		if (tkn->type == REDIR_IN || tkn->type == REDIR_OUT
			|| tkn->type == APPEND || tkn->type == HEREDOC)
		{
			if (!tkn->next)
				return (ft_perror(shell, "senut", "newline"), 2);
			else if (tkn->next->type == PIPE)
				return (ft_perror(shell, "senut", "|"), 2);
			else if (tkn->next->type == REDIR_IN || tkn->next->type == REDIR_OUT
				|| tkn->next->type == APPEND || tkn->next->type == HEREDOC)
				return (ft_perror(shell, "senut", tkn->next->value), 2);
		}
		tkn = tkn->next;
	}
	return (0);
}
