/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/21 20:48:03 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	posible problema = "'$USER"' || '"$USER'";
*/

/*la función ft_check_quotes se llama dentro de un while , mientras el input exista.
Por cada iteración (por cada carácter del input), comprobamos si la posición es una comilla.
Si no es comilla devuelve NO_QUOTES;  si hay comilla, comprobamos en el resto del input
la siguiente coincidencia con la comilla registrada. Si el input se acaba antes de encontrarla
retornamos UNCLOSED, y si el siguiente caracter de la comilla es la coincidencia,
significa que las comillas están vacias.*/

//esta funcion comprueba que la primera ocurrencia de comillas tenga su respectivo cierre,
//sin importar qué contenga en su interior, incluidas comillas de distinto tipo.
// ej.: echo "hola'".	echo "a'b""c"
//		hola'			a'bc


t_command	*ft_nodecmd(t_command *cmd, char *input, int start, int pipe)
{
	t_command	*new;
	char	*sub;

	sub = NULL;
	new = (t_command *)malloc(sizeof(t_command));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->cmd = NULL;
	new->redirs = NULL;
	new->next = NULL;
	sub = ft_substr(input, start, pipe - start);
	if (!sub)
		return (free(new), NULL);
	new->cmd = sub;
	addlastcmd_node(&cmd, new);
	return (cmd);
}

/*funcion para meter 1 comando en arg por cada vez que hay pipe o nulo.
mete lo anterior del input hasta que acaba el bucle*/
t_command	*get_command(t_command *cmd, char *input)
{
	int		is_pipe;
	int		i;

	is_pipe = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			cmd = ft_nodecmd(cmd, input, is_pipe, i);	// hay que guardar cada comando, y cada palabra por separado !!! split !!!!1
			if (!cmd)
			return (ft_error("Parse command"), NULL);
			is_pipe = i;					// que hacer con la pipe
		}
		i++;
	}
	if (input[i] == '\0')
	{
		cmd = ft_nodecmd(cmd, input, is_pipe, i);
			if (!cmd)
				return (ft_error("Parse command"), NULL);
	}
	return (cmd);
}

int	quotes_status(char *input, int *i)
{
	char	q_type;

	q_type = 0;
	if (input[*i] == '\'' || input[*i] == '\"') // Verifica si es una comilla
	{
		q_type = input[*i];
		(*i)++; // Avanza al siguiente carácter
		while (input[*i] && input[*i] != q_type)
			(*i)++;
		if (input[*i] == q_type && input[*i - 1] == q_type)
			return (EMPTY); // Comillas vacías
		if (!input[*i]) // No se encontró el cierre de la comilla
			return (UNCLOSED);
	}
	else
		return (NO_QUOTES);
	return (CLOSED);
}

char	*check_quotes(char *input)
{
	int	q_state;
	int	i;
	
	i = 0;
	q_state = 0;
	while (input[i])
	{
		q_state = quotes_status(input, &i);
		if (q_state == UNCLOSED)
		{
			input = ft_free_str(&input);
			return (ft_error("Check_quotes"), NULL);
		}
		if (q_state == EMPTY)
		{
			input = rm_empty_quotes(input, i - 1, i);
			if (!input)
				return (ft_error("empty quotes"), NULL);
			i = i - 2;
		}
/*		if (q_state == CLOSED)
		{
			input = rm_quotes(&input, i);
			if (!input)
				return (ft_error("closed quotes"), NULL);
			i = i - 2;
		} */
		i++;
	}
	return (input);
}
//comprobar si debe devolver NULL en cada caso.
//input no cambia fuera de la funcion
void	parse_input(t_shell **mshell, char *input)
{
	(*mshell)->user_input = input;
	(*mshell)->commands = get_command((*mshell)->commands, input);
	if (!(*mshell)->commands)
		return (ft_error("get command"));
	(*mshell)->p_input = check_quotes(input);
	if (!(*mshell)->p_input)
		return (ft_error("check quotes"));
	//^ check_input ^ before split into struct
	(*mshell)->commands->args = ft_split_input((*mshell)->p_input);
	if (!(*mshell)->commands->args)
		return (ft_error("split input")); //free(p_input);
	// fallo en split, cuando no hay argumentos para split ?
	// mshell> "";
	input = ft_free_str(&input);
}
