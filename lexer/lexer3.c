/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:08:05 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/20 03:56:41 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dup_redir_str(const char *start, int len)
{
	char	*redir_str;

	redir_str = ft_substr(start, 0, len);
	if (!redir_str)
		return (NULL);
	return (redir_str);
}

// Helper function to handle input redirections
t_token_type	handle_input_redir(const char **line_ptr, char **value)
{
	const char	*line = *line_ptr;

	if (*(line + 1) == '<')
	{
		*value = dup_redir_str(line, 2);
		if (!(*value))
			return (T_ERROR);
		*line_ptr += 2;
		return (T_REDIR_HEREDOC);
	}
	*value = dup_redir_str(line, 1);
	if (!(*value))
		return (T_ERROR);
	(*line_ptr)++;
	return (T_REDIR_IN);
}

// Helper function to handle output redirections
t_token_type	handle_output_redir(const char **line_ptr, char **value)
{
	const char	*line = *line_ptr;

	if (*(line + 1) == '>')
	{
		*value = dup_redir_str(line, 2);
		if (!(*value))
			return (T_ERROR);
		*line_ptr += 2;
		return (T_REDIR_APPEND);
	}
	*value = dup_redir_str(line, 1);
	if (!(*value))
		return (T_ERROR);
	(*line_ptr)++;
	return (T_REDIR_OUT);
}

// Main function to get redirection type
t_token_type	get_redir_type(const char **line_ptr, char **value)
{
	const char	*line = *line_ptr;

	if (*line == '<')
		return (handle_input_redir(line_ptr, value));
	if (*line == '>')
		return (handle_output_redir(line_ptr, value));
	return (T_ERROR);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
}
