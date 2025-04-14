/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_istrrchr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 02:10:39 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/14 02:11:02 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_istrrchr(char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	if (s[i] == c)
		return (i);
	while (s[--i])
	{
		if (s[i] == c)
			return (i);
	}
	return (-1);
}
