/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:49:22 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/07 21:37:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cleanup_interrupted_heredocs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*rd;

	cmd = shell->commands;
	while (cmd)
	{
		rd = cmd->rd;
		while (rd)
		{
			if (rd->type == HEREDOC && rd->file)
			{
				if (ft_strncmp(rd->file, "/tmp/heredoc_", 13) == 0)
				{
					unlink(rd->file);
					free(rd->file);
					rd->file = NULL;
				}
			}
			rd = rd->next;
		}
		cmd = cmd->next;
	}
}

void	validate_sigint(t_shell *mshell)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		mshell->last_exit_status = 130;
	}
	return ;
}

static void	end_of_heredoc(char *line, char *filename, t_redir *rd)
{
	if (line)
	{
		free(line);
		free(rd->file);
		rd->file = filename;
	}
	else if (g_signal_received == SIGINT)
	{
		unlink(filename);
		free(filename);
		g_signal_received = 0;
	}
	else
	{
		free(rd->file);
		rd->file = filename;
		ft_putendl_fd("minishell: heredoc ended without input", STDERR_FILENO);
	}
}

static void	expand_heredoc(t_shell *shell, char **pline)
{
	char	*line;
	int		i;

	line = *pline;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && line[i + 1] != '?')
			dollar_qu(pline, shell->last_exit_status, i);
		else if (line[i] == '$')
			dollar_expand(pline, shell->lstenv, i);
		line = *pline;
		i++;
	}
}

void	read_heredoc_lines(t_shell *shell, t_redir *rd, int fd, char *file)
{
	char	*line;

	line = readline("> ");
	while (line && ft_strcmp(line, rd->file) && g_signal_received != SIGINT)
	{
		if (line[0] == '\0')
		{
			free(line);
			line = ft_strdup("\n");
		}
		expand_heredoc(shell, &line);
		ft_putendl_fd(line, fd);
		free(line);
		line = readline("> ");
	}
	close(fd);
	end_of_heredoc(line, file, rd);
	exit(0);
}

void	clean_sigint(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_exit_status = 128 + WTERMSIG(status);
	else
		shell->last_exit_status = 0;
}
