/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:39:15 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 19:11:03 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_word(char *str, int i)
{
	char	*word;
	int		j;
	int		start;

	if (!str)
		return (NULL);
	j = 0;
	start = i;
	while (str[i] && !ft_isspace(str[i]) && !ft_isredir(str[i++]))
		j++;
	word = (char *)malloc(sizeof(char) * j + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (start < i)
		word[j++] = str[start++];
	word[j] = '\0';
	return (word);
}


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
				break;
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
	int	i;

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

/* 	ls -la
	command [0] - ls -la
	args[0][0] == "ls"
	token
		type = WORD
		value "ls"
	REDIR
	type = NULL
	value = NULL
	


		ls -l -a > txt.txt
	command [0] - ls -la > txt.txt
	args[0][0] == "ls"
	args[0][1] == "-la"
	token
		type = WORD
		value "ls"
	REDIR
	type = REDIR_OUT
	value = ">" */