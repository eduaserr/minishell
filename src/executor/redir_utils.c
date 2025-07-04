/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:01:08 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/04 13:39:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	prepare_output_files(t_shell *shell)
{
	t_redir	*redir;
	int		fd_out;

	redir = shell->commands->rd;
	while (redir)
	{
		if (redir->type == REDIR_OUT || redir->type == APPEND)
		{
			if (redir->type == APPEND)
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fd_out = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
			{
				ft_perror(shell, "minishell", redir->file);
				return (-1);
			}
			close(fd_out);
		}
		redir = redir->next;
	}
	return (0);
}

static t_redir	*find_last_output_redir(t_redir *redir_list)
{
	t_redir	*last_output_redir;

	last_output_redir = NULL;
	while (redir_list)
	{
		if (redir_list->type == REDIR_OUT || redir_list->type == APPEND)
			last_output_redir = redir_list;
		redir_list = redir_list->next;
	}
	return (last_output_redir);
}

static void	apply_output_redirection(t_redir *redir, t_shell *shell)
{
	int	fd_out;

	if (!redir)
		return ;
	if (redir->type == APPEND)
		fd_out = open(redir->file, O_WRONLY | O_APPEND);
	else
		fd_out = open(redir->file, O_WRONLY);
	if (fd_out == -1)
	{
		ft_perror(shell, "minishell", redir->file);
		ft_exit_child(&shell, EXIT_FAILURE);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
}

void	setup_redirection(t_shell *shell, bool handle_heredoc)
{
	t_redir	*last_output_redir;

	redirect_stdin(shell, handle_heredoc);
	if (!shell->commands || !shell->commands->rd)
		return ;
	if (prepare_output_files(shell) == -1)
		ft_exit_child(&shell, EXIT_FAILURE);
	last_output_redir = find_last_output_redir(shell->commands->rd);
	apply_output_redirection(last_output_redir, shell);
}
