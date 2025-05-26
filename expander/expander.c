/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:37:24 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/23 02:04:31 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *var_name, char **envp)
{
	int		i;
	size_t	name_len;

	i = 0;
	if (!var_name || !envp)
		return (NULL);
	name_len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (envp[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

char	*handle_exit_status_var(size_t *len)
{
	*len = 2;
	return (ft_strdup("?"));
}

char	*extract_braced_var(char *str, size_t *len)
{
	size_t	i;
	size_t	closing_brace_pos;

	i = 1;
	while (str[i] && str[i] != '}')
		i++;
	closing_brace_pos = i;
	if (str[closing_brace_pos] == '}')
	{
		if (!validate_braced_var_name(str, 1, closing_brace_pos))
		{
			*len = 1;
			return (NULL);
		}
		*len = closing_brace_pos + 2;
		return (safe_extract_var_substring(str, 1, closing_brace_pos - 1));
	}
	*len = 1;
	return (NULL);
}

char	*extract_simple_var(char *str, size_t *len)
{
	size_t	i;
	char	*var_name;

	i = 0;
	if (!is_valid_var_start_char(str[i]))
	{
		*len = 1;
		return (NULL);
	}
	i++;
	while (str[i] && is_valid_var_char(str[i]))
		i++;
	*len = i + 1;
	var_name = safe_extract_var_substring(str, 0, i);
	return (var_name);
}
