/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/09 19:28:25 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
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

int	ft_check_quotes(char *input, int *i)
{
	char	q_type;
	int		j;

	j = 0;
	q_type = 0;
	if (input[*i] == '\'' || input[*i] == '\"') // Verifica si es una comilla
	{
		q_type = input[*i];
		j = (*i)++; // Avanza al siguiente carácter
		while (input[*i] && input[*i] != q_type)
			(*i)++;
		ft_printf("i is -> %d\n", *i);
		if (input[j] == q_type && input[j + 1] == q_type)
			return (EMPTY); // Comillas vacías
		if (!input[*i]) // No se encontró el cierre de la comilla
			return (UNCLOSED);
	}
	else
		return (NO_QUOTES);
	return (CLOSED);
}

void	parse_input(t_shell **mshell, char *input)
{
	int	q_state;
	int	i;

	(void)mshell;
	i = 0;
	q_state = 0;
	/*while (input[i])
	{
		q_state = ft_check_quotes(input, &i);
		printf("quote_state -> %d\n", q_state);
		if (q_state == UNCLOSED)
		{
			ft_free_str(&input);
			ft_error("Check_quotes");
			//ft_free_mshell(mshell);
			return ;
		}
		if (q_state == EMPTY)
		{
			//input = ft_substr(input, );
			//rm_empty_quotes(&input);
			printf("remove_quotes\n");
		}
		i++;
	}*/
	//^ check_input ^ before split into struct
	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;
	ft_printmatrix((*mshell)->user_input);
}
