/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/09 17:00:50 by eduaserr         ###   ########.fr       */
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

int	ft_check_quotes(char *input, int i)
{
	char	quote_type;
	int		j;

	j = 0;
	quote_type = 0;
	if (ft_strchr(input, '\'') || ft_strchr(input, '\"'))
	{
		j = i;
		quote_type = input[j++];
		printf("q_type -> %c\n", quote_type);
		while (input[j] && input[j] != quote_type)
			j++;
		if (!input[j])
			return (UNCLOSED);
		if (j > 0 && input[j - 1] == quote_type) // comillas vacias "" ''
			return (EMPTY);
	}
	return (NO_QUOTES);
}

void	parse_input(t_shell **mshell, char *input)
{
	int	q_state;
	int	i;

	i = 0;
	q_state = 0;
	while (input[i])
	{
		q_state = ft_check_quotes(input, i);
		printf("q_state -> %d\n", q_state);
		printf("%d\n", i);
		if (q_state == UNCLOSED)
			return (ft_free_str(&input), ft_error("Check_quotes"), ft_free_mshell(mshell));
		if (q_state == EMPTY)
		{
			//rm_empty_quotes(&input);
			printf("remove_quotes\n");
		}
		i++;
	}
	/*^ check_input ^ before split into struct
	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;*/
}