/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:32:51 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:44:38 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_error_token(t_parser_context *ctx, t_list *tokens)
{
	t_token	*token;

	(void)ctx;
	token = (t_token *)tokens->content;
	if (token->value)
		print_error("lexer error near", token->value);
	else
		print_error("lexer error near", "");
	return (0);
}

t_list	*advance_token_pointer(t_list *tokens, t_parser_context *ctx)
{
	t_token	*current_token;

	(void)ctx;
	if (!tokens)
		return (NULL);
	current_token = (t_token *)tokens->content;
	if (is_redirection_token(current_token->type) && tokens->next)
		return (tokens->next->next);
	return (tokens->next);
}

t_list	*finalize_parser(t_parser_context *ctx)
{
	if (!ctx->current_cmd->args_list && !ctx->current_cmd->redir_list)
	{
		free_command(ctx->current_cmd);
		ctx->current_cmd = NULL;
		if (!ctx->command_list)
		{
			print_error("syntax error", "empty command pipeline");
			return (NULL);
		}
	}
	else
	{
		if (!build_argv(ctx->current_cmd))
		{
			cleanup_parser_context(ctx);
			return (NULL);
		}
		if (!add_command_to_list(ctx))
		{
			cleanup_parser_context(ctx);
			return (NULL);
		}
	}
	return (ctx->command_list);
}

t_list	*parser(t_list *tokens, int last_exit_status, char **envp)
{
	t_parser_context	ctx;

	if (!init_parser_context(&ctx, last_exit_status, envp))
		return (NULL);
	while (tokens)
	{
		if (!process_token(&ctx, tokens))
		{
			cleanup_parser_context(&ctx);
			return (NULL);
		}
		tokens = advance_token_pointer(tokens, &ctx);
	}
	return (finalize_parser(&ctx));
}
