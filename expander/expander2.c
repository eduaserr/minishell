/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:38:07 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/23 02:01:39 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_start_char(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_valid_var_char_at_position(char c, size_t pos)
{
	if (pos == 1)
		return (is_valid_var_start_char(c));
	return (is_valid_var_char(c));
}

char	*extract_var_name(char *str, size_t *len)
{
	if (!str || !*str)
		return (NULL);
	if (str[0] == '?')
		return (handle_exit_status_var(len));
	if (str[0] == '{')
		return (extract_braced_var(str, len));
	return (extract_simple_var(str, len));
}

int	validate_braced_var_name(char *str, size_t start, size_t end)
{
	size_t	i;

	if (start >= end)
		return (0);
	if (!is_valid_var_start_char(str[start]))
		return (0);
	i = start + 1;
	while (i < end)
	{
		if (!is_valid_var_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}

char	*safe_extract_var_substring(char *str, size_t start, size_t len)
{
	char	*result;

	result = ft_substr(str, start, len);
	if (!result)
		return (ft_strdup(""));
	return (result);
}
