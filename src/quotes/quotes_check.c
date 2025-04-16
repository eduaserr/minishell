/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/16 03:59:30 by eduaserr         ###   ########.fr       */
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
	char	q_type;

	q_type = 0;
	if (input[i] == '\'' || input[i] == '\"') // Verifica si es una comilla
	{
		q_type = input[i];
		i++; // Avanza al siguiente carácter
		while (input[i] && input[i] != q_type)
			i++;
		if (input[i] == q_type && input[i - 1] == q_type)
			return (EMPTY); // Comillas vacías
		if (!input[i]) // No se encontró el cierre de la comilla
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
	while (input[i])
	{
		ft_printf("1| %d\n", i);
		ft_printlines(input);
		q_state = ft_check_quotes(input, i);
		ft_printf("2| %d\n", i);
		ft_printlines(input);
	/*	if (q_state == CLOSED && get_quote(input) == '\"')
		{
			ft_printf("CLOSED y comillas\n");
			ft_printf("2.1| %d\n", i);
			input = expand_var(mshell, &input, i);
			if (!input)
				return (ft_error("expand var"));
			ft_printf("2.2| %d\n", i);
			ft_printf("%s\n", input);
			i = ft_istrchr(input, get_quote(input)) - 1;
			ft_printf("2.3| %d\n", i);
			ft_printf("%s\n", input);
			q_state = -4;
		}*/
		if (q_state == UNCLOSED)
		{
			ft_printf("UNCLOSED\n");
			ft_free_str(&input);
			ft_error("Check_quotes");
			//ft_free_mshell(mshell);
			return ;
		}
		if (q_state == EMPTY)
		{
			ft_printf("EMPTY\n");
			ft_printf("3| %d\n", i);
			ft_printlines(input);
			input = rm_empty_quotes(input, i, i + 1);
			if (!input)
			return (ft_error("empty quotes"));
			i = ft_istrchr(input, get_quote(input)) - 1;
			ft_printf("4| %d\n", i);
			ft_printlines(input);
		}
		if (q_state == CLOSED)
		{
			ft_printf("CLOSED\n");
			ft_printf("5| %d\n", i);
			ft_printlines(input);
			ft_printf("char en %c\n", input[i]);
			if (input[i] == '\'')
			{
				rm_quotes();
			}
			else if (input[i] == '\"')
			{
				//input = expand_variables(input, i); // Implementa esta función
                input = rm_quotes(&input, i);
			}
			input = rm_quotes(&input, i);
			if (!input)
				return (ft_error("Processing join"));
			i = ft_istrchr(input, get_quote(input)) - 1;
			ft_printf("6| %d\n", i);
			ft_printlines(input);
		}
		i++;
		ft_printf("end| %d\n", i);
		ft_printlines(input);
	}
	//^ check_input ^ before split into struct
/* 	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;
	ft_printmatrix((*mshell)->user_input); */
}
