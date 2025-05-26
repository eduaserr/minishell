/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3applyredirs2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:22:05 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/07 17:14:15 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Apply final dup2 operations after processing all redirections */
static int	apply_dups(int final_in, int final_out, int pipe_in, int pipe_out)
{
	if (final_in != STDIN_FILENO && dup2(final_in, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 stdin failed");
		return (0);
	}
	if (final_out != STDOUT_FILENO && dup2(final_out, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 stdout failed");
		return (0);
	}
	cleanup_fds(final_in, final_out, pipe_in, pipe_out);
	return (1);
}

/* Main redirection handler */
int	apply_redirections(t_command *cmd, int pipe_in, int pipe_out)
{
	t_list	*node;
	int		final_in;
	int		final_out;
	t_redir	*redir;
	int		success;

	node = cmd->redir_list;
	final_in = pipe_in;
	final_out = pipe_out;
	while (node)
	{
		redir = node->content;
		if (redir->type == REDIR_IN)
			success = handle_input(redir, &final_in, pipe_in);
		else if (redir->type == REDIR_HEREDOC)
			success = handle_heredoc(redir, &final_in, pipe_in);
		else if (redir->type == REDIR_OUT)
			success = handle_output(redir, &final_out, pipe_out);
		else if (redir->type == REDIR_APPEND)
			success = handle_append(redir, &final_out, pipe_out);
		if (!success)
			return (cleanup_fds(final_in, final_out, pipe_in, pipe_out), 0);
		node = node->next;
	}
	return (apply_dups(final_in, final_out, pipe_in, pipe_out));
}
