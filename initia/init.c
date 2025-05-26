/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:19:37 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 15:20:35 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	initialize_shell(t_shell_context *ctx, char **initial_envp)
{
	ctx->last_exit_status = 0;
	ctx->should_run = 1;
	ctx->actual_exit_code = 0;
	ctx->current_envp = NULL;
	ctx->current_envp = copy_environment(initial_envp);
	if (!ctx->current_envp)
		return (0);
	setup_signal_handlers();
	return (1);
}

int	is_exit_request(int exit_status)
{
	return (exit_status >= EXIT_REQUEST_SIG_OFFSET);
}

void	handle_exit_request(t_shell_context *ctx, int exit_status)
{
	ctx->actual_exit_code = exit_status - EXIT_REQUEST_SIG_OFFSET;
	ctx->should_run = 0;
	ctx->last_exit_status = ctx->actual_exit_code;
}

int	cleanup_and_exit(t_shell_context *ctx)
{
	rl_clear_history();
	free_environment(ctx->current_envp);
	return (ctx->last_exit_status);
}
