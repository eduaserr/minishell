/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 02:07:05 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/20 03:55:59 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to handle word tokens
// 1. Helper function to find the end of a word
const char	*find_word_end(const char *line, t_quote_vars *qv)
{
	qv->quoted = 0;
	qv->quote_char = 0;
	while (*line)
	{
		if (!qv->quote_char && (*line == '\'' || *line == '"'))
		{
			qv->quote_char = *line;
			qv->quoted = 1;
		}
		else if (*line == qv->quote_char)
			qv->quote_char = 0;
		else if (!qv->quote_char && (*line == ' ' || *line == '\t'
				|| *line == '<' || *line == '>' || *line == '|'))
			break ;
		line++;
	}
	return (line);
}

// 2. Helper function to create the token and handle errors
t_token	*create_word_token(char *value, t_quote_vars *qv,
		t_list **tokens)
{
	t_token	*token;

	if (qv->quoted)
	{
		value = remove_quotes(value);
		if (!value)
		{
			ft_lstclear(tokens, free_token);
			print_error("lexer", "memory allocation error");
			return (NULL);
		}
	}
	token = create_token(T_WORD, value, qv->quoted);
	if (!token || !token->value)
	{
		free(value);
		if (token)
			free_token(token);
		ft_lstclear(tokens, free_token);
		print_error("lexer", "memory allocation failed");
		return (NULL);
	}
	return (token);
}

// 3. Main function to handle word tokens
t_list	*handle_word(const char **line_ptr, t_list **tokens)
{
	const char		*start = *line_ptr;
	const char		*end;
	char			*value;
	t_token			*token;
	t_quote_vars	qv;

	end = find_word_end(*line_ptr, &qv);
	value = ft_substr(start, 0, end - start);
	if (!value)
	{
		ft_lstclear(tokens, free_token);
		print_error("lexer", "memory allocation error");
		return (NULL);
	}
	token = create_word_token(value, &qv, tokens);
	if (!token)
		return (NULL);
	ft_lstadd_back(tokens, ft_lstnew(token));
	*line_ptr = end;
	return (*tokens);
}

// Helper function to add the end-of-file token
t_list	*add_eof_token(t_list **tokens)
{
	t_token	*token;

	token = create_token(T_EOF, NULL, 0);
	if (!token)
	{
		ft_lstclear(tokens, free_token);
		print_error("lexer", "memory allocation failed for EOF");
		return (NULL);
	}
	ft_lstadd_back(tokens, ft_lstnew(token));
	return (*tokens);
}

// Main lexer function
t_list	*lexer(const char *line)
{
	t_list	*tokens;
	char	quote_char;

	tokens = NULL;
	quote_char = 0;
	skip_whitespace(&line);
	while (*line)
	{
		skip_whitespace(&line);
		if (!*line)
			break ;
		if ((*line == '<' || *line == '>') && !handle_redir(&line, &tokens))
			return (NULL);
		else if (*line == '|' && !handle_pipe(&line, &tokens))
			return (NULL);
		else if (!(*line == '<' || *line == '>' || *line == '|')
			&& !handle_word(&line, &tokens))
			return (NULL);
	}
	if (quote_char != 0)
	{
		ft_lstclear(&tokens, free_token);
		return (print_error("syntax error", "memalloc failed for EOF"), NULL);
	}
	return (add_eof_token(&tokens));
}
