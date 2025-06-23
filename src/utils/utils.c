/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:18:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/23 14:41:55 by aamoros-         ###   ########.fr       */
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

char	*get_word_msh(char *str, int i)
{
	char	*word;
	int		j;
	int		start;

	if (!str)
		return (NULL);
	j = 0;
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		i++;
		j++;
	}
	word = (char *)malloc(sizeof(char) * j + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (start < i)
		word[j++] = str[start++];
	word[j] = '\0';
	return (word);
}

void	ft_free_array(void **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
