/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 18:38:45 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/23 02:02:54 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_content_from_quotes(char *str, int quotes_type)
{
	size_t	content_len;

	content_len = ft_strlen(str);
	if (quotes_type != 0 && content_len >= 2)
		return (ft_substr(str, 1, content_len - 2));
	else
		return (ft_strdup(str));
}

char	*process_variable_expansion(char *content, int last_exit_status,
		char **envp)
{
	t_expansion_context	ctx;
	char				*result;
	size_t				i;
	size_t				start;
	size_t				content_len;

	ctx.last_exit_status = last_exit_status;
	ctx.envp = envp;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = -1;
	start = 0;
	content_len = ft_strlen(content);
	while (++i < content_len)
	{
		if (content[i] == '$')
		{
			result = append_literal_text(result, content, start, i);
			i = process_dollar_expansion(content, &result, i, &ctx);
			start = i + 1;
		}
	}
	result = append_literal_text(result, content, start, i);
	return (result);
}

char	*append_literal_text(char *result, char *content, size_t start,
		size_t end)
{
	char	*literal_part;

	if (start >= end)
		return (result);
	literal_part = ft_substr(content, start, end - start);
	if (!literal_part)
		return (result);
	result = ft_strjoin_free(result, literal_part);
	return (result);
}

size_t	process_dollar_expansion(char *content, char **result, size_t pos,
		t_expansion_context *ctx)
{
	char	*var_name;
	char	*var_value;
	size_t	var_len;
	size_t	chars_consumed;

	var_name = extract_var_name(&content[pos + 1], &var_len);
	if (var_name)
	{
		var_value = get_variable_value(var_name, ctx->last_exit_status,
				ctx->envp);
		*result = ft_strjoin_free(*result, var_value);
		free(var_name);
		chars_consumed = var_len - 1;
	}
	else
	{
		var_value = ft_substr(content, pos, 1);
		*result = ft_strjoin_free(*result, var_value);
		chars_consumed = 0;
	}
	return (pos + chars_consumed);
}

char	*get_variable_value(char *var_name, int last_exit_status, char **envp)
{
	char	*temp;
	char	*var_value;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(last_exit_status));
	temp = get_env_value(var_name, envp);
	if (temp)
		var_value = ft_strdup(temp);
	else
		var_value = ft_strdup("");
	return (var_value);
}

char	*expand_string(char *str, int last_exit_status, char **envp,
		int quotes_type)
{
	char	*content_to_expand;
	char	*result;

	if (!str)
		return (ft_strdup(""));
	content_to_expand = extract_content_from_quotes(str, quotes_type);
	if (!content_to_expand)
		return (ft_strdup(""));
	if (quotes_type == 1)
		return (content_to_expand);
	result = process_variable_expansion(content_to_expand, last_exit_status,
			envp);
	free(content_to_expand);
	return (result);
}
