/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/08 00:37:16 by eduaserr         ###   ########.fr       */
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

static t_cmd	*ft_nodecmd(t_shell *shell, t_cmd *cmd, char *input, int *ix)
{
	t_cmd	*new;

	new = NULL;
	new = create_cmd(new);
	if (!new)
		return (NULL);
	new->cmd = parse_cmd(input, ix[0], ix[1]);
	if (!new->cmd)
		return (free(new), ft_free_cmd(&cmd), ft_error("trim input"), NULL);
	new->rd = redir_node(shell, shell->tkn, new->rd, shell->cmd_count);
	addlastcmd_node(&cmd, new);
	return (cmd);
}

static int	find_next_pipe(char *input, int start)
{
	int	i;

	i = start;
	while (input[i])
	{
		if (skip_quoted(input, &i))
			continue ;
		if (input[i] == '|')
			return (i);
		i++;
	}
	return (i);
}

static t_cmd	*process_pipe_segment(t_shell *mshell, t_cmd *cmd, 
		char *input, int *ix)
{
	cmd = ft_nodecmd(mshell, cmd, input, ix);
	if (!cmd)
		return (NULL);
	mshell->cmd_count++;
	return (cmd);
}

t_cmd	*get_cmd(t_shell *mshell, t_cmd *cmd, char *input, int is_pipe)
{
	int	pos[2];

	pos[0] = is_pipe;
	while (input[pos[0]])
	{
		pos[1] = find_next_pipe(input, pos[0]);
		if (input[pos[1]] == '|')
		{
			cmd = process_pipe_segment(mshell, cmd, input, pos);
			if (!cmd)
				return (ft_error("Parse command"), NULL);
			pos[0] = pos[1] + 1;
		}
		else
			break ;
	}
	cmd = ft_nodecmd(mshell, cmd, input, pos);
	if (!cmd)
		return (ft_error("Parse last command"), NULL);
	return (cmd);
}
