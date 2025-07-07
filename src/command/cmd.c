/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/07 15:30:22 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*parse_cmd(char *input, int start, int pipe)
{
	char	*sub;
	char	*tmp;

	tmp = NULL;
	sub = NULL;
	sub = ft_substr(input, start, pipe - start);
	if (!sub)
		return (NULL);
	tmp = ft_strtrim(sub, " \t\n\r\v\f");
	ft_free_str(&sub);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (ft_error("syntax error near unexpected token `|'"),
			ft_free_str(&tmp));
	return (tmp);
}

static t_cmd	*ft_nodecmd(t_shell *shell, t_cmd *cmd, char *input, int start,
		int pipe)
{
	t_cmd	*new;

	new = NULL;
	new = create_cmd(new);
	if (!new)
		return (NULL);
	new->cmd = parse_cmd(input, start, pipe);
	if (!new->cmd)
		return (free(new), ft_free_cmd(&cmd), ft_error("trim input"), NULL);
	new->rd = redir_node(shell, shell->tkn, new->rd, shell->cmd_count);
	addlastcmd_node(&cmd, new);
	return (cmd);
}

t_cmd	*get_cmd(t_shell *mshell, t_cmd *cmd, char *input, int is_pipe)
{
	int	i;

	i = -1;
	mshell->cmd_count = 0;
	while (input[++i])
	{
		if (skip_quoted(input, &i))
			continue ;
		if (input[i] == '|')
		{
			cmd = ft_nodecmd(mshell, cmd, input, is_pipe, i);
			if (!cmd)
				return (ft_error("Parse command"), NULL);
			is_pipe = i + 1;
			mshell->cmd_count++;
		}
	}
	if (input[i] == '\0')
	{
		cmd = ft_nodecmd(mshell, cmd, input, is_pipe, i);
		if (!cmd)
			return (ft_error("Parse last command"), NULL);
	}
	return (cmd);
}
