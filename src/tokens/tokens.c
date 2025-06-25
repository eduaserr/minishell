/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:39:15 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/25 23:09:49 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	set_typeq(char *str, int *type, int i)
{
	if (str[i] == '\"')
		*type = DOUBLE;
	else if (str[i] == '\'')
		*type = SIMPLE;
}

static t_token	*get_tkn_word(t_token *new, char *str, int *i)
{
	char	*word;
	int		start;
	int		j;

	start = *i;
	j = *i;
	while (str[j] && !ft_isspace(str[j]) && !ft_isredir(str[j]))
	{
		if (str[j] == '\'' || str[j] == '\"')
		{
			if (!skip_quoted(str, &j))
				break ;
		}
		else
			j++;
	}
	word = ft_substr(str, start, j - start);
	if (!word)
		return (NULL);
	new = get_token(new, WORD, word, i);
	set_typeq(str, &new->type, start);
	ft_free_str(&word);
	return (new);
}

static t_token	*parse_tkn(t_token *new, char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
		new = get_token(new, HEREDOC, "<<", i);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		new = get_token(new, APPEND, ">>", i);
	else if (input[*i] == '<')
		new = get_token(new, REDIR_IN, "<", i);
	else if (input[*i] == '>')
		new = get_token(new, REDIR_OUT, ">", i);
	else if (input[*i] == '|')
		new = get_token(new, PIPE, "|", i);
	else if (input[*i])
		new = get_tkn_word(new, input, i);
	if (!new)
		return (ft_error("new is NULL"), NULL);
	return (new);
}

t_token	*tokenizer(t_token *token_list, char *input)
{
	t_token	*new;
	int		i;

	new = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (!input[i])
			return (token_list);
		new = parse_tkn(new, input, &i);
		if (!new)
			return (ft_free_tkn(&token_list), ft_error("parse token"), NULL);
		addlast_tknnode(&token_list, new);
	}
	return (token_list);
}
