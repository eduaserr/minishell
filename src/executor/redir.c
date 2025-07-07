/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:09:29 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 19:51:32 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_redir	*find_last_input_redir(t_redir *redir_list)
{
	t_redir	*last_input_redir;

	last_input_redir = NULL;
	while (redir_list)
	{
		if (redir_list->type == REDIR_IN || redir_list->type == HEREDOC)
			last_input_redir = redir_list;
		redir_list = redir_list->next;
	}
	return (last_input_redir);
}

static void	process_heredocs(t_cmd *command,
		t_redir *last_input_redir)
{
	t_redir	*current_redir;
	int		heredoc_fd;

	current_redir = command->rd;
	while (current_redir)
	{
		if (current_redir->type == HEREDOC)
		{
			heredoc_fd = open(current_redir->file, O_RDONLY);
			if (current_redir == last_input_redir)
				dup2(heredoc_fd, STDIN_FILENO);
			close(heredoc_fd);
		}
		current_redir = current_redir->next;
	}
}

static void	apply_file_input(t_shell *shell, char *file)
{
	int	fd_in;

	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
	{
		ft_perror(shell, "nsfod", shell->commands->rd->file);
		ft_exit_child(&shell, EXIT_FAILURE);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
}

void	redirect_stdin(t_shell *shell, bool handle_heredoc)
{
	t_cmd		*command;
	t_redir		*last_input_redir;

	command = shell->commands;
	if (!command || !command->rd)
		return ;
	last_input_redir = find_last_input_redir(command->rd);
	if (!last_input_redir)
		return ;
	if (handle_heredoc)
		process_heredocs(command, last_input_redir);
	if (last_input_redir->type == REDIR_IN)
		apply_file_input(shell, last_input_redir->file);
}
