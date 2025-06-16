/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 20:37:56 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		handle_pipes_err(char *str, int i)
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
	ft_free_str(&input);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')		//intro con solo espacios
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

void	parse_commands(t_shell **mshell)
{
	(*mshell)->commands = get_command(*mshell, (*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (ft_error("get cmd"));
	get_args((*mshell)->tkn, (*mshell)->commands);
	if (!(*mshell)->commands->args)
		return (ft_error("get cmd args"));
	dup_cmd((*mshell)->commands);
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
	if (handle_pipes_err((*mshell)->p_input, 0)) // handle_reddir
		return (ft_error("syntax error near unexpected token `|'"));
	parse_commands(mshell);
	// parse_redirecciones
	// si es tkn->type DOUBLE ("") checkear por expansión
	// si hay $ checkear siguiente posicion para expandir
	// si es variable del sistema , sustituir
		//^ check_input ^ before split into struct
}


/*Necesitas implementar la expansión antes de eliminar las comillas.
Actualmente tienes expand_var en quotes_expand.c pero no se está usando.
*/