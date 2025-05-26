/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:40:10 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 17:30:06 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main execution entry point */
int	execute(t_list *cmds, char ***envp)
{
	const int	count = ft_lstsize(cmds);
	pid_t		*pids;
	int			status;
	t_exec_ctx	ctx;

	if (!cmds || count == 0)
		return (0);
	if (!init_execution(&pids, count, cmds, *envp))
		return (EXIT_GENERAL_ERROR);
	if (count == 1 && try_single_builtin(cmds, envp, pids, &status))
		return (status);
	else
	{
		ctx = (t_exec_ctx){cmds, pids, count, STDIN_FILENO, {-1, -1}, 0, envp};
		status = process_pipeline(&ctx);
	}
	free(pids);
	return (status);
}

/* Initialization helper */
bool	init_execution(pid_t **pids, int count, t_list *cmds, char **envp)
{
	const int	hd_status = handle_heredocs(cmds, 0, envp);

	*pids = malloc(sizeof(pid_t) * count);
	if (!*pids)
		return (false);
	if (hd_status <= 0)
	{
		free(*pids);
		return (false);
	}
	return (true);
}

/* Single builtin handler */
bool	try_single_builtin(t_list *cmd_node, char ***envp, pid_t *pids,
		int *status)
{
	t_command	*cmd;
	char		*name;
	const char	*builtins[] = {"cd", "export", "unset", "exit", NULL};

	cmd = cmd_node->content;
	name = cmd->argv[0];
	if (!is_builtin(name) || !str_in_array(name, builtins))
		return (false);
	*status = exec_parent_builtin(cmd, envp);
	free(pids);
	return (true);
}

/* Builtin execution in parent process */
int	exec_parent_builtin(t_command *cmd, char ***envp)
{
	int	original[2];
	int	redir_status;
	int	status;

	original[0] = dup(STDIN_FILENO);
	original[1] = dup(STDOUT_FILENO);
	redir_status = apply_redirections(cmd, STDIN_FILENO, STDOUT_FILENO);
	if (redir_status)
		status = execute_builtin(cmd, envp, 0);
	else
		status = EXIT_FAILURE;
	restore_fds(original);
	return (status);
}

/* Pipeline processor */
int	process_pipeline(t_exec_ctx *ctx)
{
	t_command	*cmd;
	bool		has_next;

	while (ctx->cmd_idx < ctx->num_cmds)
	{
		has_next = ctx->cmd_idx < ctx->num_cmds - 1;
		cmd = ctx->cmds->content;
		if (!setup_pipe(ctx, has_next))
			return (cleanup_pipeline(ctx, EXIT_GENERAL_ERROR));
		if (!execute_command(ctx, cmd, has_next))
			return (cleanup_pipeline(ctx, EXIT_GENERAL_ERROR));
		update_pipeline_state(ctx, has_next);
		ctx->cmd_idx++;
		ctx->cmds = ctx->cmds->next;
	}
	return (wait_for_children(ctx->pids, ctx->num_cmds));
}

void	restore_fds(int original[2])
{
	if (original[0] != -1)
	{
		dup2(original[0], STDIN_FILENO);
		close(original[0]);
	}
	if (original[1] != -1)
	{
		dup2(original[1], STDOUT_FILENO);
		close(original[1]);
	}
}
