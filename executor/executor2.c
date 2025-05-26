/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:40:50 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/19 14:52:30 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Pipe management */
bool	setup_pipe(t_exec_ctx *ctx, bool has_next)
{
	if (has_next && pipe(ctx->pipe_fds) == -1)
		return (false);
	return (true);
}

/* Command execution */
bool	execute_command(t_exec_ctx *ctx, t_command *cmd, bool has_next)
{
	int	out_fd;

	if (has_next)
		out_fd = ctx->pipe_fds[1];
	else
		out_fd = STDOUT_FILENO;
	ctx->pids[ctx->cmd_idx] = execute_single_command(cmd, ctx->envp,
			ctx->prev_read, out_fd);
	return (ctx->pids[ctx->cmd_idx] != -1);
}

/* State updates */
void	update_pipeline_state(t_exec_ctx *ctx, bool has_next)
{
	close_if_needed(ctx->prev_read, STDIN_FILENO);
	close_if_needed(ctx->pipe_fds[1], STDOUT_FILENO);
	if (has_next)
		ctx->prev_read = ctx->pipe_fds[0];
	else
		ctx->prev_read = STDIN_FILENO;
}

/* Cleanup helper */
int	cleanup_pipeline(t_exec_ctx *ctx, int status)
{
	close_if_needed(ctx->prev_read, STDIN_FILENO);
	close_if_needed(ctx->pipe_fds[0], STDIN_FILENO);
	close_if_needed(ctx->pipe_fds[1], STDOUT_FILENO);
	free(ctx->pids);
	return (status);
}

/* Child process waiter */
int	wait_for_children(pid_t *pids, int count)
{
	int	last_status;
	int	status;
	int	i;

	last_status = 0;
	i = 0;
	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
		{
			i++;
			continue ;
		}
		if (i == count - 1)
			last_status = interpret_exit_status(status);
		i++;
	}
	return (last_status);
}



void	close_if_needed(int fd, int std_fd)
{
	if (fd != std_fd)
		close(fd);
}


int	interpret_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE); // fallback
}

bool str_in_array(const char *str, const char **array)
{
    int i;
    
    if (!str || !array)
        return (false);
        
    i = 0;
    while (array[i] != NULL)
    {
        if (ft_strcmp(str, array[i]) == 0)
            return (true);
        i++;
    }
    return (false);
}
