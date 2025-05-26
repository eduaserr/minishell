/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:10:31 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:40:17 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_parser_context(t_parser_context *ctx, int last_exit_status,
		char **envp)
{
	ctx->command_list = NULL;
	ctx->last_exit_status = last_exit_status;
	ctx->envp = envp;
	ctx->error_occurred = 0;
	ctx->current_cmd = ft_calloc(1, sizeof(t_command));
	if (!ctx->current_cmd)
		return (0);
	return (1);
}

void	cleanup_parser_context(t_parser_context *ctx)
{
	if (ctx->current_cmd)
		free_command(ctx->current_cmd);
	if (ctx->command_list)
		ft_lstclear(&ctx->command_list, free_command);
}

int	process_token(t_parser_context *ctx, t_list *tokens)
{
	t_token	*token;

	token = (t_token *)tokens->content;
	if (token->type == T_PIPE)
		return (handle_pipe_token(ctx, tokens));
	else if (is_redirection_token(token->type))
		return (handle_redirection_token(ctx, tokens));
	else if (token->type == T_WORD)
		return (handle_word_token(ctx, tokens));
	else if (token->type == T_ERROR)
		return (handle_error_token(ctx, tokens));
	return (1);
}

int	handle_pipe_token(t_parser_context *ctx, t_list *tokens)
{
	t_token	*token;
	t_token	*next_token;

	token = (t_token *)tokens->content;
	next_token = get_next_token(tokens);
	if (!ctx->current_cmd->args_list)
	{
		print_error("syntax error near unexpected token", token->value);
		return (0);
	}
	if (!build_argv(ctx->current_cmd))
		return (0);
	if (!add_command_to_list(ctx))
		return (0);
	if (!create_new_current_cmd(ctx))
		return (0);
	if (!next_token || next_token->type == T_PIPE)
	{
		print_error("syntax error near unexpected token", token->value);
		return (0);
	}
	return (1);
}
