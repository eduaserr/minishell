/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/19 03:05:54 by eduaserr         ###   ########.fr       */
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
	sub = ft_free_str(&sub);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (free(tmp), ft_error("syntax error near unexpected token `|'"), NULL);
	return (tmp);
}

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
	//Por qué free_cmd aqui? porque el return NULL hace que yo no pueda liberar
	//el cmd fuera de esta función ya que return NULL asigna NULL a la lista (cmd = NULL)
	//y pierdo las referencias antes de poder liberar desde fuera (process_input)
	//parse_redirs(&new, mshell->tkn);
	new->rd = redir_node(mshell->tkn, new->rd);
	if (!new->rd)
		return (free(new->cmd), free(new), ft_free_cmd(&cmd), ft_error("parse redir"), NULL);
	addlastcmd_node(&cmd, new);
	return (cmd);
}

t_command	*get_command(t_shell *mshell, t_command *cmd, char *input)
{
	int		is_pipe;
	int		i;

	is_pipe = 0;
	i = 0;
	while (input[i])
	{
		if (skip_quoted(input, &i))
			continue ;
		if (input[i] == '|')
		{
			cmd = ft_nodecmd(mshell, cmd, input, is_pipe, i);	// hay que guardar cada comando, y cada palabra por separado !!! split !!!!1
			if (!cmd)
				return (ft_error("Parse command"), NULL);
			is_pipe = i + 1;					// que hacer con la pipe
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
		while (tkn && tkn->value[0] != '|' && i < len)
		{
			cmd->args[i] = ft_strdup(tkn->value);
			if (!cmd->args[i])
				return (ft_freematrix(&cmd->args), ft_error("strdup"));
			i++;
			tkn = tkn->next;
		}
		cmd->args[i] = NULL;
		if (tkn && tkn->value[0] == '|')
			tkn = tkn->next;
		cmd = cmd->next;
	}
}
