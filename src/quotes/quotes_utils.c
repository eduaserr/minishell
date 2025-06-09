/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:21:13 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/09 13:16:29 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		get_quote(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			return ('\'');
		if (str[i] == '\"')
			return ('\"');
		i++;
	}
	return (-1);
}

/*Start -> posición 1º comilla
End -> posición 2º comilla*/
char	*get_in_quotes(char *str, int start, int end)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	len = end - start - 1;
	if (start < 0)
		return (NULL);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	while (str[++start] && (--len) >= 0)
	{
		tmp[i] = str[start];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

/* Esta función actúa cuando las comiilas estan vacias "". Recoge lo anterior y lo posterior
de las comillas del input y las joinea. --> hola""adios --> holaadios || ""
si no hay input anterior ni posterior? Devuelve un input vacio a la funcion parse y esta lo
vuelve a procesar check_quotes() esta vez sin comillas.
*/
char	*rm_empty_quotes(char *str, int start, int end)
{
	char	*s1;
	char	*s2;

	s1 = ft_substr(str, 0, start);
	if (!s1)
		return (ft_error("sub 1"), NULL);
	s2 = ft_substr(str, end + 1, ft_strlen(str));
	if (!s2)
		return (free(s1), ft_error("sub 2"), NULL);
	free(str);
	str = NULL;
	str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (str);
}

char	*rm_quotes(char **input, int i)
{
	char	c;
	char	*tmp;
	int		j;
	int		q;

	q = 0;
	c = (*input)[i];
	i = 0;
	j = 0;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(*input) - 2 + 1));
	if (!tmp)
		return (NULL);
	while ((*input)[i])
	{
		if ((*input)[i] == c && q < 2)
		{
			q++;
			i++;
		}
		tmp[j++] = (*input)[i++];
	}
	tmp[j] = '\0';
	free(*input);
	*input = NULL;
	return (tmp);
}
