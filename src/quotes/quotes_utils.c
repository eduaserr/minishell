/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 06:21:13 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/18 23:52:46 by eduaserr         ###   ########.fr       */
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
	return (0);
}

//no se usa
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

char	*rm_quotes(char *input, int i, int end)
{
	char	*tmp;
	int		j;
	int		start;

	j = 0;
	start = 0;
	tmp = (char *)malloc(sizeof(char) * (ft_strlen(input) - 1));
	if (!tmp)
		return (NULL);
	while (input[start])
	{
		if (start == i || start == end)
			start++;
		else
			tmp[j++] = input[start++];
	}
	tmp[j] = '\0';
	return (tmp);
}

char *rm_quotes2(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = NULL;
	while (str[i])
	{
		j = i;
		if (skip_quoted(str, &i))
		{
			tmp = rm_quotes(str, j, i - 1);
				if (!tmp)
					return (NULL);
			ft_free_str(&str);
			str = tmp;
			i = i - 2;
		}
		else
			i++;
	}
	return (str);
}
