/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/15 22:06:52 by eduaserr         ###   ########.fr       */
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

static int	quotes_status(char *input, int *i)
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
			return (ft_error("unclosed quotes"), NULL);
		}
		if (q_state == EMPTY)
		{
			input = rm_empty_quotes(input, i - 1, i);
			if (!input)
				return (ft_error("empty quotes"), NULL);
			i = i - 2;
		}
		i++;
	}
	return (input);
}
