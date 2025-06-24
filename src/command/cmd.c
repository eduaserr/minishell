/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/24 23:46:57 by eduaserr         ###   ########.fr       */
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
	new->rd = redir_node(mshell->tkn, new->rd, mshell->cmd_count);
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
		ft_printf("pipelen -> HOW MANY ARGS  =  %d\n", len);
		cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
		if (!cmd->args)
			return (ft_error("malloc cmd->args"));
		while (tkn && tkn->type != PIPE && i < len)
		{
			cmd->args[i] = ft_strdup(tkn->value);
			if (!cmd->args[i])
				return (ft_freematrix(&cmd->args), ft_error("strdup"));
			i++;
			tkn = tkn->next;
		}
		cmd->args[i] = NULL;
		if (tkn->type == REDIR_IN || tkn->type == REDIR_OUT)
			tkn = tkn->next->next;
		if (tkn && tkn->type == PIPE)
			tkn = tkn->next;
		cmd = cmd->next;
	}
}
