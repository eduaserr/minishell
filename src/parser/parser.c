/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/26 12:51:12 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_pipes_err(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '|' && i == 0)
			return (1);
		if (str[i] == '|' && str[i + 1] == '|')
			return (1);
		i++;
	}
	if (i > 0 && str[i - 1] == '|')
		return (1);
	return (0);
}

/* char *get_redir_symbol(t_token_type type)
{
	if (type == REDIR_IN)
		return ("<");
	if (type == REDIR_OUT)
		return (">");
	if (type == APPEND)
		return (">>");
	if (type == HEREDOC)
		return ("<<");
	return ("token");
}

int	handle_rd_err(t_token *tkn)
{
	t_token *swp;

	swp = tkn;
	while (swp)
	{
		if ((swp->type == REDIR_IN || swp->type == REDIR_OUT
			|| swp->type == APPEND || swp->type == HEREDOC) && !swp->next)
			return (1);
		if ((swp->type == REDIR_IN || swp->type == REDIR_OUT
			|| swp->type == APPEND) && (swp->next->type == REDIR_IN
				|| swp->next->type == REDIR_OUT || swp->next->type == APPEND))
			return (1);
		swp = swp->next;
	}
	return (0);
} */

int	handle_rd_err(t_token *tkn)
{
	while (tkn)
	{
		if (tkn->type == REDIR_IN || tkn->type == REDIR_OUT
			|| tkn->type == APPEND || tkn->type == HEREDOC)
		{
			if (!tkn->next)
				return (ft_error("syntax error near unexpected token `newline'\n"), 1);
			if (tkn->next->type == PIPE)
				return (ft_error("syntax error near unexpected token `|'\n"), 1);
			if (tkn->next->type == REDIR_IN || tkn->next->type == REDIR_OUT
				|| tkn->next->type == APPEND || tkn->next->type == HEREDOC)
				return (ft_printf("%ssyntax error near unexpected token `%s'\n",
					"\x1b[31mError :\x1B[37m ", tkn->next->value), 1);
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

int	validate_redir(t_command *cmd)
{
	t_redir	*rd;

	while (cmd)
	{
		rd = cmd->rd;
		while (rd)
		{
			if (!rd->file || rd->file[0] == '\0')
				return (1);
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	parse_commands(t_shell **mshell)
{
	(*mshell)->commands = get_command(*mshell, (*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (ft_error("get cmd"));
	if (validate_redir((*mshell)->commands))
		return (ft_error("ambiguous redirect"));
	get_args((*mshell)->tkn, (*mshell)->commands);
	if (!(*mshell)->commands->args)
		return (ft_error("get cmd args"));
	dup_cmd((*mshell), (*mshell)->commands);
}

void	parse_input(t_shell **mshell, char *input)
{
	(*mshell)->p_input = preparate_input(input);
	if (!(*mshell)->p_input)
		return ;
	//parse_tokens
	(*mshell)->tkn = tokenizer((*mshell)->tkn, (*mshell)->p_input);
	if (!(*mshell)->tkn)
		return (ft_error("token"));
	if (handle_pipes_err((*mshell)->p_input, 0))
		return (ft_error("syntax error near unexpected token `|'"));
	if (handle_rd_err((*mshell)->tkn)) // handle_reddir
		return ;
	parse_commands(mshell);
	// parse_redirecciones
		//^ check_input ^ before split into struct
}
