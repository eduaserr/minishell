/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:11:21 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/23 03:11:52 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*create_redirection(t_token *token, t_token *next_token,
		t_parser_context *ctx)
{
	t_redir	*redir;
	char	*expanded_value;
	int		quotes_type;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	quotes_type = detect_quote_type(next_token);
	expanded_value = expand_string(next_token->value, ctx->last_exit_status,
			ctx->envp, quotes_type);
	if (!expanded_value)
	{
		free(redir);
		return (NULL);
	}
	redir->type = (t_redir_type)(token->type - T_REDIR_IN);
	redir->value = expanded_value;
	redir->is_value_quoted = (quotes_type != 0);
	redir->heredoc_fd = -1;
	return (redir);
}

int	add_redirection_to_command(t_command *cmd, t_redir *redir)
{
	t_list	*new_node;

	new_node = ft_lstnew(redir);
	if (!new_node)
		return (0);
	ft_lstadd_back(&cmd->redir_list, new_node);
	return (1);
}

int	handle_word_token(t_parser_context *ctx, t_list *tokens)
{
	t_token	*token;
	char	*expanded_value;
	int		quotes_type;

	token = (t_token *)tokens->content;
	quotes_type = detect_quote_type(token);
	expanded_value = expand_string(token->value, ctx->last_exit_status,
			ctx->envp, quotes_type);
	if (!expanded_value)
		return (0);
	if (!add_argument_to_command(ctx->current_cmd, expanded_value))
	{
		free(expanded_value);
		return (0);
	}
	return (1);
}

int	add_argument_to_command(t_command *cmd, char *arg)
{
	t_list	*new_node;

	new_node = ft_lstnew(arg);
	if (!new_node)
		return (0);
	ft_lstadd_back(&cmd->args_list, new_node);
	return (1);
}

int	detect_quote_type(t_token *token)
{
	if (!token->quoted || ft_strlen(token->value) < 2)
		return (0);
	if (token->value[0] == '\'')
		return (1);
	else if (token->value[0] == '"')
		return (2);
	return (0);
}
