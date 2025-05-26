/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections3applyredirs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:21:29 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/07 17:14:28 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Helper to clean up file descriptors */
void	cleanup_fds(int final_in, int final_out, int pipe_in, int pipe_out)
{
	if (final_in != pipe_in)
		close(final_in);
	if (final_out != pipe_out)
		close(final_out);
}

/* Handle input redirection (<) */
int	handle_input(t_redir *redir, int *final_in, int pipe_in)
{
	const int	fd = open(redir->value, O_RDONLY);

	if (fd == -1)
	{
		print_error_detail(redir->value, NULL, strerror(errno));
		return (0);
	}
	if (*final_in != pipe_in)
		close(*final_in);
	*final_in = fd;
	return (1);
}

/* Handle heredoc redirection (<<) */
int	handle_heredoc(t_redir *redir, int *final_in, int pipe_in)
{
	if (redir->heredoc_fd < 0)
		return (0);
	if (*final_in != pipe_in)
		close(*final_in);
	*final_in = redir->heredoc_fd;
	return (1);
}

/* Handle output redirection (>) */
int	handle_output(t_redir *redir, int *final_out, int pipe_out)
{
	const int	fd = open(redir->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1)
	{
		print_error_detail(redir->value, NULL, strerror(errno));
		return (0);
	}
	if (*final_out != pipe_out)
		close(*final_out);
	*final_out = fd;
	return (1);
}

/* Handle append redirection (>>) */
int	handle_append(t_redir *redir, int *final_out, int pipe_out)
{
	const int	fd = open(redir->value, O_WRONLY | O_CREAT | O_APPEND, 0644);

	if (fd == -1)
	{
		print_error_detail(redir->value, NULL, strerror(errno));
		return (0);
	}
	if (*final_out != pipe_out)
		close(*final_out);
	*final_out = fd;
	return (1);
}
