/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:07:12 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/20 02:09:12 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to handle redirection tokens
t_list	*handle_redir(const char **line_ptr, t_list **tokens)
{
	t_token			*token;
	t_token_type	type;
	char			*value;

	type = get_redir_type((const char **)line_ptr, &value);
	if (type == T_ERROR)
	{
		ft_lstclear(tokens, free_token);
		print_error("lexer", "redirection error");
		return (NULL);
	}
	token = create_token(type, value, 0);
	if (!token || !token->value)
	{
		free(value);
		free_token(token);
		ft_lstclear(tokens, free_token);
		print_error("lexer", "memory allocation failed");
		return (NULL);
	}
	ft_lstadd_back(tokens, ft_lstnew(token));
	return (*tokens);
}

// Helper function to handle pipe tokens
t_list	*handle_pipe(const char **line_ptr, t_list **tokens)
{
	t_token	*token;
	char	*value;

	(*line_ptr)++;
	value = ft_strdup("|");
	token = create_token(T_PIPE, value, 0);
	if (!token || !token->value)
	{
		free(value);
		free_token(token);
		ft_lstclear(tokens, free_token);
		print_error("lexer", "memory allocation failed");
		return (NULL);
	}
	ft_lstadd_back(tokens, ft_lstnew(token));
	return (*tokens);
}

void	free_token(void *token_ptr)
{
	t_token	*token;

	token = (t_token *)token_ptr;
	if (token)
	{
		free(token->value);
		free(token);
	}
}

t_token	*create_token(t_token_type type, char *value, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quoted = quoted;
	return (token);
}
