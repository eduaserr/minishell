/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:18:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 20:30:49 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int		ft_isredir(int a)
{
	return (a == '<' || a == '>' || a == '|');
}

int		skip_quoted(char *str, int *i)
{
	int	q;

	q = 0;
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		q = str[(*i)++];
		while (str[*i] && str[*i] != q)
			(*i)++;
		if (str[*i] == q)
			(*i)++;
		return (1);		//Comillas saltadas
	}
	else
		return (0);		//No habia comillas
}

char	**ft_init_array(char **array)
{
	char	**tmp;
	int		i;

	i = 0;
	if (!array || !*array)
		return (NULL);
	while (array && array[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (array[i])
	{
		tmp[i] = ft_strdup(array[i]);
		if (!tmp[i])
		{
			ft_freematrix(&tmp);
			return (NULL);
		}
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}
