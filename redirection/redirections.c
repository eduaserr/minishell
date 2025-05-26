/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:06:50 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/15 20:00:28 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Close two file descriptors, ignoring errors for -1
void	close_fds(int fd1, int fd2)
{
	if (fd1 >= 0)
		close(fd1);
	if (fd2 >= 0)
		close(fd2);
}

// Process all heredocs for a command list *before* other redirections
static int	process_heredoc_redirection(t_redir *redir, int last_exit_status,
		char **envp)
{
	if (redir->type != REDIR_HEREDOC)
		return (0);
	if (redir->heredoc_fd >= 0)
		close(redir->heredoc_fd);
	redir->heredoc_fd = read_heredoc_to_pipe(redir, last_exit_status, envp);
	if (redir->heredoc_fd == -2)
		return (-1);
	if (redir->heredoc_fd < 0)
		return (1);
	return (0);
}

int	handle_heredocs(t_list *command_list, int last_exit_status, char **envp)
{
	t_list		*cmd_node;
	t_command	*cmd;
	t_list		*redir_node;
	t_redir		*redir;
	int			proc_result;

	cmd_node = command_list;
	while (cmd_node)
	{
		cmd = (t_command *)cmd_node->content;
		redir_node = cmd->redir_list;
		while (redir_node)
		{
			redir = (t_redir *)redir_node->content;
			proc_result = process_heredoc_redirection(redir, last_exit_status,
					envp);
			if (proc_result == -1)
				return (-1);
			if (proc_result == 1)
				return (print_error("heredoc", "FD not available"), 0);
			redir_node = redir_node->next;
		}
		cmd_node = cmd_node->next;
	}
	return (1);
}

// Restore original stdin and stdout, close saved FDs
void	restore_redirections(int *original_fds)
{
	if (original_fds[0] != -1)
	{
		dup2(original_fds[0], STDIN_FD);
		close(original_fds[0]);
		original_fds[0] = -1;
	}
	if (original_fds[1] != -1)
	{
		dup2(original_fds[1], STDOUT_FD);
		close(original_fds[1]);
		original_fds[1] = -1;
	}
}
