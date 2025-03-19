/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:18:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/19 22:29:29 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**ft_init_array(char **array)
{
	char	**tmp;
	int		i;

	i = 0;
	if (!array)
		return (NULL);
	while (array && array[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * i + 1);
	if (!tmp)
		return (NULL);
	i = 0;
	while (array[i])
	{
		tmp[i] = ft_strdup(array[i]);
		if (!tmp[i])
		{
			ft_freemap(tmp);
			return (NULL);
		}
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}
