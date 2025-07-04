/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/04 14:02:44 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_pipes_err(char *str, int i)
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

static int	handle_rd_err(t_shell *shell, t_token *tkn)
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

static char	*preparate_input(char *input)
{
	char	*tmp;

	tmp = ft_strtrim(input, " \t\n\r\v\f");
	ft_free_str(&input);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (ft_free_str(&tmp));
	tmp = check_quotes(tmp);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (ft_free_str(&tmp));
	input = ft_strtrim(tmp, " \t\n\r\v\f");
	ft_free_str(&tmp);
	if (!input)
		return (NULL);
	if (input[0] == '\0')
		return (ft_free_str(&input));
	return (input);
}

static int	validate_redir(t_cmd *cmd)
{
	t_redir	*rd;

	while (cmd)
	{
		rd = cmd->rd;
		while (rd)
		{
			if (!rd->file || rd->file[0] == '\0')
				return (SYNTAX_ERROR_STATUS);
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	parse_commands(t_shell **mshell, int tmp)
{
	(*mshell)->commands = get_cmd(*mshell,
		(*mshell)->commands, (*mshell)->p_input, 0);
	if (!(*mshell)->commands)
		return (ft_error("get cmd"));
	tmp = validate_redir((*mshell)->commands);
	if (tmp != 0 && tmp != (*mshell)->last_exit_status)
	{
		(*mshell)->last_exit_status = tmp;
		return (ft_error("ambiguous redirect"));
	}
	get_args((*mshell)->tkn, (*mshell)->commands);
	if (!(*mshell)->commands->args)
		return (ft_error("get cmd args"));
	dup_cmd((*mshell), (*mshell)->commands);
}

void	parse_input(t_shell **mshell, char *input)
{
	int	tmp;

	tmp = 0;
	(*mshell)->p_input = preparate_input(input);
	if (!(*mshell)->p_input)
		return ;
	(*mshell)->tkn = tokenizer((*mshell)->tkn, (*mshell)->p_input);
	if (!(*mshell)->tkn)
		return (ft_error("token"));
	if (handle_pipes_err((*mshell)->p_input, 0))
		return (ft_perror(*mshell, "senut", "|"));
	if (handle_rd_err(*mshell, (*mshell)->tkn))
		return ;
	parse_commands(mshell, tmp);
}
