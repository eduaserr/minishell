/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:11:47 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 19:20:16 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*get_word(char *str, int i)
{
	char	*word;
	int		j;
	int		start;

	if (!str)
		return (NULL);
	j = 0;
	start = i;
	while (str[i] && !ft_isspace(str[i++]))
		j++;
	word = (char *)malloc(sizeof(char) * j + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (start < i)
		word[j++] = str[start++];
	word[j] = '\0';
	return (word);
}
