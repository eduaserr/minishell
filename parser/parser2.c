/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:10:56 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:40:23 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_token(t_list *tokens)
{
	if (!tokens || !tokens->next || !tokens->next->content)
		return (NULL);
	return ((t_token *)tokens->next->content);
}

int	add_command_to_list(t_parser_context *ctx)
{
	t_list	*new_node;

	new_node = ft_lstnew(ctx->current_cmd);
	if (!new_node)
		return (0);
	ft_lstadd_back(&ctx->command_list, new_node);
	ctx->current_cmd = NULL;
	return (1);
}

int	create_new_current_cmd(t_parser_context *ctx)
{
	ctx->current_cmd = ft_calloc(1, sizeof(t_command));
	if (!ctx->current_cmd)
		return (0);
	return (1);
}

int	handle_redirection_token(t_parser_context *ctx, t_list *tokens)
{
	t_token	*token;
	t_token	*next_token;
	t_redir	*redir;

	token = (t_token *)tokens->content;
	next_token = get_next_token(tokens);
	if (!validate_redirection_syntax(token, next_token))
		return (0);
	redir = create_redirection(token, next_token, ctx);
	if (!redir)
		return (0);
	if (!add_redirection_to_command(ctx->current_cmd, redir))
	{
		free_redir(redir);
		return (0);
	}
	return (1);
}

int	validate_redirection_syntax(t_token *token, t_token *next_token)
{
	if (!next_token || next_token->type != T_WORD)
	{
		print_error("syntax error near unexpected token", token->value);
		return (0);
	}
	return (1);
}
