/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 16:45:13 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/04 03:17:05 by eduaserr         ###   ########.fr       */
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

static int	wordcount(char const *s, char c)
{
	int		n_word;
	int		i;

	i = -1;
	n_word = 0;
	while (s[++i])
	{
		if (s[i] != c)
		{
			n_word++;
			while (s[i] != c && s[i + 1] != '\0')
				i++;
		}
	}
	return (n_word);
}

char	**ft_mshell_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		start;
	int		end;

	i = -1;
	start = 0;
	end = 0;
	str = ft_calloc(wordcount(s, c) + 1, sizeof(char *));
	if (!str || !s)
		return (NULL);
	while (++i < wordcount(s, c) && s[start])
	{
		while (s[start] && s[start] == c)
			start++;
		end = ft_strchr(s + start, c) - s;
		if (end > ft_strlen(s) || end < 0)
			end = ft_strlen(s);
		str[i] = ft_substr(s, start, end - start);
		if (!str[i])
			return (free_split(str, i));
		start = end;
	}
	str[i] = NULL;
	return (str);
}

int	countwords(char *str)
{
	int		i;
	int		count;
	char	quote;

	count = 0;
	i = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
		{
			if (str[i] == '"' || str[i] == '\'')
			{
				quote = str[i];
				i++;
				while (str[i] && str[i] != quote)
					i++;
				if (str[i] == quote)
					i++;
			}
			else
			{
				while (str[i] && !ft_isspace(str[i]) && str[i] != '"' && str[i] != '\'')
					i++;
			}
			count++;
		}
	}
	return (count);
}

char	*ft_getword(char *str, int start, int end)
{
	char	*word;
	int		j;

	j = 0;
	word = (char *)malloc((sizeof(char) * (end - start) + 1));
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

char	**ft_split2(char *str)
{
	int		i = 0;
	int		j = 0;
	int		start = 0;
	int		end = 0;
	int		nwords = countwords(str);
	char	**arr;
	char	quote;

	quote = 0;
	arr = (char **)malloc(sizeof(char *) * (nwords + 1));
	if (!arr)
		return (NULL);
	while (str[i] && j < nwords)
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		if (str[i])
		{
			start = i;
			if (str[i] == '"' || str[i] == '\'')
			{
				quote = str[i++];
				while (str[i] && str[i] != quote)
					i++;
				if (str[i] == quote)
					i++;
			}
			else
			{
				while (str[i] && !ft_isspace(str[i]) && str[i] != '"' && str[i] != '\'')
					i++;
			}
			end = i - 1;
			arr[j] = ft_getword(str, start, end);
			if (!arr[j])
			{
				while (j > 0)
					free(arr[--j]);
				free(arr);
				return (NULL);
			}
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
	input = ft_split2(str);
	if (!input)
		return (ft_error("split error"), NULL);
	return (input);
}
