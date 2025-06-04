/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:45:13 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/04 19:53:47 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	**free_split(char **str, int i)
{
	while (--i >= 0)
		free(str[i]);
	free(str);
	return (NULL);
}

static int	many_words(char *str, int *i, int quote)
{
	int	count;

	count = 0;
	if (str[*i])
	{
		if (str[*i] == '\"' || str[*i] == '\'')
		{
			quote = str[(*i)++];
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				(*i)++;
		}
		else
		{
			while (str[*i] && !ft_isspace(str[*i])
				&& str[*i] != '\"' && str[*i] != '\'')
				(*i)++;
		}
		count++;
	}
	return (count);
}

int	countwords(char *str)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		count += many_words(str, &i, quote);
	}
	return (count);
}

char	*ft_getword(char *str, int start, int end)
{
	char	*word;
	int		j;

	j = 0;
	word = (char *)malloc((sizeof(char) * (end - start + 1) + 1));
	if (!word)
		return (NULL);
	while (str[start] && (start <= end))
	{
		word[j] = str[start];
		j++;
		start++;
	}
	word[j] = '\0';
	return (word);
}

static char	*getword(char *str, int *i)
{
	int	start;
	int	end;
	int	quote;

	end = 0;
	start = *i;
	if (str[*i] == '\"' || str[*i] == '\'')
	{
		quote = str[(*i)++];
		while (str[*i] && str[*i] != quote)
			(*i)++;
		if (str[*i] == quote)
			(*i)++;
	}
	else
	{
		while (str[*i] && !ft_isspace(str[*i]) && str[*i] != '\"' && str[*i] != '\'')
			(*i)++;
	}
	end = *i - 1;
	return (ft_getword(str, start, end));
}

char	**ft_mshell_split(char *str)
{
	int		i;
	int		j;
	int		nwords;
	char	**arr;

	nwords = countwords(str);
	arr = (char **)malloc(sizeof(char *) * (nwords + 1));
	if (!arr)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && j < nwords)
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
		{
			arr[j] = getword(str, &i);
			if (!arr[j])
				return (free_split(arr, j));
			j++;
		}
	}
	arr[j] = NULL;
	return (arr);
}

char	**ft_split_input(char *str)
{
	char	**input;

	input = NULL;
	if (!str)
		return (NULL);
	input = ft_mshell_split(str);
	if (!input)
		return (ft_error("split error"), NULL);
	return (input);
}
