/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/04 03:05:54 by eduaserr         ###   ########.fr       */
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




/* 
*/

t_command	*create_cmd(char **sub)
{
	t_command	*new;

	new->args = ft_arrdup(sub); //meterlos 1 a 1? o tras procesar el input
	if (!new)
		return (NULL);
	new->redirs = NULL;
	new->next = NULL;
	return (new);
}

t_command	*ft_nodecmd(t_command *cmd, char *input, int pipe)
{
	char	*sub;
	char	**arrsub;

	arrsub = NULL;
	sub = NULL;
	cmd = NULL;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	sub = ft_substr(input, 0, pipe);
	if (!sub)
		return (NULL);

	//check_quotes , check_redir
	//sub = check_quotes(sub); // esta funcion podria continuarse ddesde el main?
	//if (!sub)
	//	return (NULL);
	//cmd = create_cmd(sub);
	//if (!cmd)
	//	return (NULL);
	//addlast_node(&cmd); //hacer funcion con t_cmd
	return (cmd);
}

/*funcion para meter 1 comando en arg por cada vez que hay pipe o nulo.
mete lo anterior del input hasta que acaba el bucle*/
void	get_command(t_command *cmd, char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|' || input[i] == '\0')
		{
			cmd = ft_nodecmd(cmd, input, i); // hay que guardar cada palabra por separado !!! split !!!!1
			if (!cmd)
				return (ft_error(0));
		}
		i++;
	}
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
			//ft_free_mshell(mshell);
			return (ft_error("Check_quotes"), NULL);
		}
		if (q_state == EMPTY)
		{
			input = rm_empty_quotes(input, i - 1, i);
			if (!input)
				return (ft_error("empty quotes"), NULL);
		}
		if (q_state == CLOSED)
		{
			input = rm_quotes(&input, i);
			if (!input)
				return (ft_error("closed quotes"), NULL);
			i = i - 2;
		}
		i++;
	}
	return (input);
}
//comprobar si debe devolver NULL en cada caso.

void	parse_input(t_shell **mshell, char *input)
{
	get_command((*mshell)->commands, input);
	(*mshell)->input = check_quotes(input);
	if (!(*mshell)->input)
		return (free(input));
	//^ check_input ^ before split into struct
	(*mshell)->user_input = ft_split_input((*mshell)->input);
	if (!(*mshell)->user_input)
		return (free(input));
	if ((*mshell)->user_input)
	{
		ft_printmatrix((*mshell)->user_input);
		ft_freematrix(&(*mshell)->user_input);
	}
	input = ft_free_str(&input);
}
//sobran free();