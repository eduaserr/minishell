/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:16:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:36:15 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free_node(t_command **new)
{
	if (!*new)
		return ;
	if ((*new)->redirs)
		free((*new)->redirs); // añadir función de liberar redirs
	if ((*new)->cmd)
		ft_free_str(&(*new)->cmd);
	free(*new);
	*new = NULL;
}

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

static t_command	*ft_nodecmd(t_command *cmd, char *input, int start, int pipe)
{
	t_command	*new;

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
	/* new->redirs = parse_redirs(new->cmd);
	if (!new->redirs)
		return (free(new->cmd), free(new), ft_free_cmd(&cmd), ft_error("parse redir"), NULL); */
	new->args = ft_split_input(new->cmd);
	if (!new->args)
		return (ft_free_node(&new), ft_free_cmd(&cmd), ft_error("split input"), NULL);
	addlastcmd_node(&cmd, new);
	return (cmd);
}

/*funcion para meter 1 comando en arg por cada vez que hay pipe o nulo.
mete lo anterior del input hasta que acaba el bucle

primero guardar y luego comprobar?*/

t_command	*get_command(t_command *cmd, char *input)
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
			cmd = ft_nodecmd(cmd, input, is_pipe, i);	// hay que guardar cada comando, y cada palabra por separado !!! split !!!!1
			if (!cmd)
				return (ft_error("Parse command"), NULL);
			is_pipe = i + 1;					// que hacer con la pipe
		}
		i++;
	}
	if (input[i] == '\0')
	{
		cmd = ft_nodecmd(cmd, input, is_pipe, i);
			if (!cmd)
				return (ft_error("Parse last command"), NULL);
	}
	return (cmd);
}