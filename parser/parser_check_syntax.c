/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_syntax.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:08:13 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:59:42 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_syntax_state(t_syntax_state *state)
{
	state->last_was_operator = 1;
	state->expecting_word_after_redir = 0;
}

t_token_type	get_token_type(t_list *token_node)
{
	t_token	*token;

	if (!token_node || !token_node->content)
		return (T_EOF);
	token = (t_token *)token_node->content;
	return (token->type);
}

t_token	*get_token(t_list *token_node)
{
	if (!token_node || !token_node->content)
		return (NULL);
	return ((t_token *)token_node->content);
}

int	validate_current_token(t_list *tokens, t_syntax_state *state)
{
	t_token			*current_token;
	t_token			*next_token;
	t_token_type	current_type;
	t_token_type	next_type;

	current_token = get_token(tokens);
	next_token = get_token(tokens->next);
	current_type = current_token->type;
	if (next_token)
		next_type = next_token->type;
	else
		next_type = T_EOF;
	if (is_operator_token(current_type))
		return (validate_operator_token(current_token, next_token, state));
	else if (current_type == T_WORD)
		return (validate_word_token(current_token, state));
	return (1);
}

int	validate_operator_token(t_token *current, t_token *next,
		t_syntax_state *state)
{
	t_token_type	current_type;
	t_token_type	next_type;

	current_type = current->type;
	if (next)
		next_type = next->type;
	else
		next_type = T_EOF;
	if (is_operator_token(next_type))
	{
		print_syntax_error(next->value);
		return (0);
	}
	if (current_type == T_PIPE)
		return (validate_pipe_token(current, next_type, state));
	if (is_redirection_token(current_type))
		return (validate_redirection_token(current, next_type, state));
	state->last_was_operator = 1;
	return (1);
}
