/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:04:44 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 18:35:39 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*create_heredoc_filename(char *suffix)
{
	char	*filename;

	filename = ft_strjoin("/tmp/heredoc_", suffix);
	free(suffix);
	if (!filename)
		ft_error("minishell: Memory allocation failed for heredoc filename.");
	return (filename);
}

static void	handle_heredoc_child(t_shell *shell, t_redir *rd, char *filename)
{
	int	fd;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		exit(1);
	read_heredoc_lines(shell, rd, fd);
}

static int	handle_heredoc_parent(t_shell *shell, t_redir *rd, char *filename,
		pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	clean_sigint(shell, status);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		unlink(filename);
		free(filename);
		g_signal_received = SIGINT;
		return (1);
	}
	free(rd->file);
	rd->file = filename;
	return (0);
}

static void	create_heredoc_file(t_shell *shell, t_redir *rd, char *n)
{
	char	*filename;
	pid_t	pid;

	filename = create_heredoc_filename(n);
	if (!filename)
		ft_exit(&shell);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(filename);
		return ;
	}
	else if (pid == 0)
		handle_heredoc_child(shell, rd, filename);
	else if (handle_heredoc_parent(shell, rd, filename, pid))
		return ;
}

void	create_heredocs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*rd;
	int		n;

	setup_heredoc_signals();
	cmd = shell->commands;
	while (cmd && g_signal_received != SIGINT)
	{
		n = 0;
		rd = cmd->rd;
		while (rd && g_signal_received != SIGINT)
		{
			if (rd->type == HEREDOC)
				create_heredoc_file(shell, rd, ft_itoa(n++));
			rd = rd->next;
		}
		cmd = cmd->next;
	}
	if (g_signal_received == SIGINT)
		cleanup_interrupted_heredocs(shell);
}
