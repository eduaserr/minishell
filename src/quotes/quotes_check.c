/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/05 13:52:35 by eduaserr         ###   ########.fr       */
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
/* t_redir	*parse_redirs(char *cmd)
{
	if (ft_strchr(cmd, '<'))
	{}
} */

void	ft_free_node(t_command **new)
{
	if (!*new)
		return ;
	if ((*new)->redirs)
		free((*new)->redirs);
	if ((*new)->cmd)
		ft_free_str(&(*new)->cmd);
	free(*new);
	*new = NULL;
}

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

char	*parse_cmd(char *input, int start, int pipe)
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

t_command	*ft_nodecmd(t_command *cmd, char *input, int start, int pipe)
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
		return (free(new->cmd), free(new), ft_error("parse redir"), NULL); */
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
		//if (comilla)
		//	itera hasta que cierre o encuentr coincidencia
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
	while (input && input[i])
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

char	*preparate_input(char *input)
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
		return (free(input), ft_error_exit(mshell, "process input", 0));	//free_mshell. Por algun motivo no hace falta?. no hace falta porqe el bucle vuelve y salgo con ctrl + D? necesito función de errores
	
	if (handle_pipes_err((*mshell)->p_input, 0)) // handle_reddir
		return (free(input), ft_error_exit(mshell, "syntax error near unexpected token `|'", 0));
	(*mshell)->commands = get_command((*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (free(input), ft_error_exit(mshell, "get command", 0));		//free_mshell
		//^ check_input ^ before split into struct
	// fallo en split, cuando no hay argumentos para split ?
	// mshell> "";
	input = ft_free_str(&input);
}
