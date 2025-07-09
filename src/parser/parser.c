/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/07 18:40:10 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*preparate_input(t_shell *shell, char *input)
{
	char	*tmp;

	tmp = ft_strtrim(input, " \t\n\r\v\f");
	ft_free_str(&input);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (ft_free_str(&tmp));
	tmp = check_quotes(shell, tmp);
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
	(*mshell)->commands = get_cmd(*mshell, (*mshell)->commands,
			(*mshell)->p_input, 0);
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
	(*mshell)->p_input = preparate_input(*mshell, input);
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
