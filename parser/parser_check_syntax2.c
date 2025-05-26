/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_syntax2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:09:31 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:55:52 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_pipe_token(t_token *current, t_token_type next_type,
		t_syntax_state *state)
{
	if (next_type == T_EOF)
	{
		print_syntax_error("|");
		return (0);
	}
	if (state->last_was_operator)
	{
		print_syntax_error(current->value);
		return (0);
	}
	state->last_was_operator = 1;
	return (1);
}

int	validate_redirection_token(t_token *current, t_token_type next_type,
		t_syntax_state *state)
{
	(void)current;
	if (next_type != T_WORD)
	{
		if (next_type == T_EOF)
			print_syntax_error("newline");
		else
			print_syntax_error("unexpected token");
		return (0);
	}
	state->last_was_operator = 1;
	state->expecting_word_after_redir = 1;
	return (1);
}

int	validate_word_token(t_token *current, t_syntax_state *state)
{
	(void)current;
	state->last_was_operator = 0;
	state->expecting_word_after_redir = 0;
	return (1);
}

int	validate_final_state(t_syntax_state *state)
{
	if (state->expecting_word_after_redir)
	{
		print_syntax_error("newline");
		return (0);
	}
	return (1);
}

int	check_syntax(t_list *tokens)
{
	t_syntax_state	state;

	if (!tokens || !tokens->content)
		return (1);
	 init_syntax_state(&state);
	while (tokens && get_token_type(tokens) != T_EOF)
	{
		if (!validate_current_token(tokens, &state))
			return (0);
		tokens = tokens->next;
	}
	return (validate_final_state(&state));
}
