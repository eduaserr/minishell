/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_in_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 19:02:23 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/09 15:21:39 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

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
