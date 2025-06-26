/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 23:59:03 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/26 12:55:35 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	swp_value(char **input, char *value, int i, int end)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
	s1 = ft_substr(*input, 0, i);
	s2 = ft_substr(*input, end + 1, ft_strlen(*input));
	ft_free_str(input);
	*input = ft_strjoin(s1, value);
	free(s1);
	*input = ft_strjoin_gnl(*input, s2);
	free(s2);
}

static int	get_var_end(char *str, int start)
{
	int		i;

	i = start;
	if (str[i] != '$')
		return (start);
	i++;
	if (!ft_isalnum(str[i]) && str[i] != '_')
		return (start);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i - 1);
}

int	dollar_expand(char **str, t_env *env, int i)
{
	char	*value;
	char	*word;
	int		end;

	word = NULL;
	end = get_var_end(*str, i);
	if (end > i)
	{
		word = get_word_msh(*str, i + 1);
		if (word)
		{
			value = ft_getenv(env, word);
			if (!value)
				value = ft_strdup("");
			swp_value(str, value, i, end);
			i += ft_strlen(value) - 1;
			ft_free_str(&word);
			ft_free_str(&value);
		}
	}
	return (i);
}
