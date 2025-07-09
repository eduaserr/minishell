/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rm_db_spaces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 04:24:56 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/09 17:22:15 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_strlendb(char *str, int i)
{
	int	len;
	int	space;

	space = 1;
	len = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			if (!space)
			{
				len++;
				space = 1;
			}
		}
		else
		{
			len++;
			space = 0;
		}
		i++;
	}
	if (len > 0 && ft_isspace(str[i - 1]))
		len--;
	return (len);
}

static char	*ft_strcpydb(char *str, char *tmp, int i, int len)
{
	int	j;
	int	space;

	space = 1;
	j = 0;
	while (str[i] && j < len)
	{
		if (ft_isspace(str[i]))
		{
			if (!space)
			{
				tmp[j++] = ' ';
				space = 1;
			}
		}
		else
		{
			tmp[j++] = str[i];
			space = 0;
		}
		i++;
	}
	tmp[j] = '\0';
	return (tmp);
}

char	*ft_rm_db_spaces(char **str)
{
	char	*tmp;
	int		i;
	int		len;
	int		space;

	if (!str || !*str)
		return (NULL);
	i = 0;
	len = 0;
	space = 1;
	len = ft_strlendb(*str, i);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	tmp = ft_strcpydb(*str, tmp, i, len);
	ft_free_str(str);
	return (tmp);
}
