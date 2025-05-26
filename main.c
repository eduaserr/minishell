/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:31:09 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 15:21:23 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_shell_loop(t_shell_context *ctx)
{
	char	*line;

	while (ctx->should_run)
	{
		g_signal_received = 0;
		line = get_user_input(ctx);
		if (!line)
		{
			handle_eof_input(ctx);
			continue ;
		}
		if (is_empty_line(line))
		{
			handle_empty_line(ctx, line);
			continue ;
		}
		process_command_line(ctx, line);
		free(line);
	}
}

char	*get_user_input(t_shell_context *ctx)
{
	char	*line;

	line = readline(PROMPT);
	if (g_signal_received == SIGINT)
		ctx->last_exit_status = EXIT_SIGINT;
	return (line);
}

void	process_command_line(t_shell_context *ctx, char *line)
{
	t_list	*tokens;
	t_list	*command_list;

	add_history(line);
	tokens = tokenize_input(line);
	if (!tokens)
	{
		ctx->last_exit_status = EXIT_GENERAL_ERROR;
		return ;
	}
	command_list = parse_tokens(tokens, ctx);
	ft_lstclear(&tokens, free_token);
	if (!command_list)
	{
		ctx->last_exit_status = SYNTAX_ERROR_STATUS;
		return ;
	}
	execute_commands(ctx, command_list);
	ft_lstclear(&command_list, free_command);
}

void	execute_commands(t_shell_context *ctx, t_list *command_list)
{
	int	execution_result;

	execution_result = execute(command_list, &ctx->current_envp);
	if (is_exit_request(execution_result))
	{
		handle_exit_request(ctx, execution_result);
		return ;
	}
	ctx->last_exit_status = execution_result;
}

int	main(int argc, char **argv, char **initial_envp)
{
	t_shell_context	ctx;

	(void)argc;
	(void)argv;
	if (!initialize_shell(&ctx, initial_envp))
		return (EXIT_FAILURE);
	run_shell_loop(&ctx);
	return (cleanup_and_exit(&ctx));
}
