/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/26 17:29:02 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*static void	ft_free_node(t_command **new)
{
	if (!*new)
		return ;
	if ((*new)->redirs)
		free((*new)->redirs); // añadir función de liberar redirs
	if ((*new)->cmd)
		ft_free_str(&(*new)->cmd);
	free(*new);
	*new = NULL;
}*/

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
		return (ft_error("syntax error near unexpected token `|'"), ft_free_str(&tmp));
	return (tmp);
}

// pasar mshell y coger de ahí el contenido. (ten cuidado con no perder la referencia
// de las cabezas de las listas, o de modificar el contenido cuando no debes)
static t_command	*ft_nodecmd(t_shell *mshell, t_command *cmd, char *input, int start, int pipe)
{
	t_command	*new;

	(void)mshell;
	new = NULL;
	new = create_cmd(new);
	if (!new)
		return (NULL);
	new->cmd = parse_cmd(input, start, pipe);
	if (!new->cmd)
		return (free(new), ft_free_cmd(&cmd), ft_error("trim input"), NULL);
	new->rd = redir_node(mshell, mshell->tkn, new->rd, mshell->cmd_count);
	addlastcmd_node(&cmd, new);
	return (cmd);
}

t_command	*get_command(t_shell *mshell, t_command *cmd, char *input)
{
	int		is_pipe;
	int		i;

	is_pipe = 0;
	i = 0;
	mshell->cmd_count = 0;
	while (input[i])
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
		i++;
	}
	if (input[i] == '\0')
	{
		cmd = ft_nodecmd(mshell, cmd, input, is_pipe, i);
		if (!cmd)
			return (ft_error("Parse last command"), NULL);
	}
	return (cmd);
}

void	get_args(t_token *tkn, t_command *cmd)
{
	int			i;
	int			len;

	while (cmd)
	{
		i = 0;
		len = pipelen(tkn);
		cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
		if (!cmd->args)
			return (ft_error("malloc cmd->args"));
		while (tkn && tkn->type != PIPE && i < len)
		{
			if ((tkn->type == REDIR_IN || tkn->type == REDIR_OUT || tkn->type == HEREDOC
				|| tkn->type == APPEND) && (tkn->next->type == WORD || tkn->next->type == DOUBLE
					|| tkn->next->type == SIMPLE))
				tkn = tkn->next->next;
			else
			{
				cmd->args[i] = ft_strdup(tkn->value);
				if (!cmd->args[i])
					return (ft_freematrix(&cmd->args), ft_error("strdup"));
				i++;
				tkn = tkn->next;
			}
		}
		cmd->args[i] = NULL;
		while (tkn && tkn->type != PIPE)
			tkn = tkn->next;
		if (tkn && tkn->type == PIPE)
			tkn = tkn->next;
		cmd = cmd->next;
	}
}
