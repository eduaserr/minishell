/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 18:04:01 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/14 04:41:19 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
	    dest[i] = src[i];
	    i++;
	}
	dest[i] = '\0';
	return (dest);
}
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
/* Esta función actúa cuando las comiilas estan vacias "". Recoge lo anterior y lo posterior
de las comillas del input y las joinea. --> hola""adios --> holaadios || ""
si no hay input anterior ni posterior? Devuelve un input vacio a la funcion parse y esta lo
vuelve a procesar check_quotes() esta vez sin comillas.

int start -> i - 1. primera comilla
int end -> i. segunda comilla

depende del funcionamiento de rm_empty_quotes,
deberia ser : end_str1 = i - 1; start_str2 = i + 1;
*/

char	*rm_empty_quotes(char *str, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(str, 0, start - 1);
	if (!s1)
		return (ft_error("sub 1"), NULL);
	s2 = ft_substr(str, end, ft_strlen(str));
	if (!s2)
		return (free(s1), ft_error("sub 2"), NULL);
	ft_printf("s1 ---> %s\n", s1);
	ft_printf("s2 ---> %s\n", s2);
	ft_printf("str ---> %s\n", str);
	free(str);
	str = NULL;
	str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (str);
}

char	*rm_quotes(char *input, int i)
{
	char	c;
	char	*tmp;

	c = input[i];
	i = 0;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(input) - 2 + 1));
	if (!tmp)
	return (NULL);
	while (input[i])
	{
		if (input[i] != c)
			tmp[i] = input[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
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
		ft_printf("1| i = %d\n", i);
		q_state = ft_check_quotes(input, &i);
		ft_printf("2| i = %d\n", i);
		if (q_state == UNCLOSED)
		{
			ft_free_str(&input);
			ft_error("Check_quotes");
			//ft_free_mshell(mshell);
			return ;
		}
		if (q_state == EMPTY)
		{
			input = rm_empty_quotes(input, i - 1, i);
			if (!input)
			return (ft_error("empty quotes"));
			i = 0;
			printf("remove_quotes\n");
		}
		if (q_state == CLOSED)
		{
			
			ft_printf("CLOSED\n");
			input = rm_quotes(input, i);
			if (!input)
			return (ft_error("Processing join"));
		}
		ft_printf("input -> %s\n", input);
		i++;
	}
	//^ check_input ^ before split into struct
/* 	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;
	ft_printmatrix((*mshell)->user_input); */
}
