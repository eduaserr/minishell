/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:28:12 by eduaserr         ###   ########.fr       */
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
		// esto lo puedo comprobar en la linea guardada del comando
	if (i > 0 && str[i - 1] == '|')// Check for pipe at the end
		return (1);
	return (0);
}

static char	*preparate_input(char *input)
{
	char	*tmp;

	tmp = ft_strtrim(input, " \t\n\r\v\f");
	if (!tmp)
		return (free(input), NULL);
	if (tmp[0] == '\0')		//intro con solo espacios
		return (free(tmp), free(input), NULL);
	input = ft_free_str(&input);
	input = tmp;
	tmp = NULL;
	return (input);
}

void	parse_input(t_shell **mshell, char *input)
{
	input = preparate_input(input);
	if (!input)
		return ;
	(*mshell)->p_input = check_quotes(ft_strtrim(input, " \t\n\r\v\f")); // strtrim hace malloc
	if (!(*mshell)->p_input)
		return (free(input), ft_error_exit(mshell, "process input", 0));
	//parse_tokens
	(*mshell)->tkn = tokenizer((*mshell)->tkn, (*mshell)->p_input);
	if (!(*mshell)->tkn)
		return (free(input), ft_error("token"));
	if (handle_pipes_err((*mshell)->p_input, 0)) // handle_reddir
		return (free(input), ft_error_exit(mshell, "syntax error near unexpected token `|'", 0));
	//split por comillas¿
	 (*mshell)->commands = get_command((*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (free(input), ft_error_exit(mshell, "get command", 0));		//free_mshell
		//^ check_input ^ before split into struct
	input = ft_free_str(&input);
}
