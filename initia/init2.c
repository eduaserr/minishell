/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:20:52 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 15:21:27 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*tokenize_input(char *line)
{
	return (lexer(line));
}

t_list	*parse_tokens(t_list *tokens, t_shell_context *ctx)
{
	return (parser(tokens, ctx->last_exit_status, ctx->current_envp));
}

void	handle_eof_input(t_shell_context *ctx)
{
	write(STDERR_FD, "exit\n", 5);
	ctx->should_run = 0;
}

int	is_empty_line(char *line)
{
	return (!line || !*line);
}

void	handle_empty_line(t_shell_context *ctx, char *line)
{
	if (line)
		ctx->last_exit_status = 0;
	free(line);
}
