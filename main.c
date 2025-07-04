/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/04 14:40:57 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}


void	end_of_heredoc(char *line, char *filename, t_redir *rd)
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

void	expand_heredoc(t_shell *shell, char **pline)
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

void	create_heredoc_file(t_shell *shell, t_redir *rd, char *n)
{
	char	*filename;
	char	*line;
	int		fd;

	filename = ft_strjoin("/tmp/heredoc_", n);
	free(n);
	if (!filename)
	{
		ft_error("minishell: Memory allocation failed for heredoc filename.");
		ft_exit(&shell);
	}
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		ft_error("minishell: Failed to open heredoc file.");
		free(filename);
		ft_exit(&shell);
	}
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
	end_of_heredoc(line, filename, rd);
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
		while (rd)
		{
			if (rd->type == HEREDOC)
				create_heredoc_file(shell, rd, ft_itoa(n++));
			rd = rd->next;
		}
		cmd = cmd->next;
	}
}

static void	execute_command_pipeline(t_shell *shell)
{
	int	pid;
	int	status;

	sync_env_array(shell);
	create_heredocs(shell);
	if (g_signal_received == SIGINT)
		return ;
	handle_exec_sigquit();
	if (execute_parent_builtin(shell->commands->args, shell))
		return ;
	pid = fork();
	if (pid == -1)
	{
		ft_error("minishell: fork");
		return ;
	}
	if (pid == 0)
		execute(shell, shell->commands->args, shell->env);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_exit_status = 128 + WTERMSIG(status);
		else
			shell->last_exit_status = 0;
	}
}

void	remove_heredocs(t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = shell->commands;
	while (cmd)
	{
		redir = cmd->rd;
		while (redir)
		{
			if (redir->type == HEREDOC && redir->file)
				unlink(redir->file);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

static void	process_command_line(t_shell *shell)
{
	char	*input;

	input = promp_input(shell);
	if (!input)
	{
		ft_exit(&shell);
		return ;
	}
	if (input[0] != '\0')
	{
		parse_input(&shell, ft_strdup(input));
		if (shell->commands)
		{
			execute_command_pipeline(shell);
			remove_heredocs(shell);
			signal_function();
		}
		update_shell(&shell);
	}
	free(input);
}

static void	run_main_loop(t_shell *shell)
{
	while (shell->running)
	{
		process_command_line(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	int		exit_code;

	(void)argc;
	(void)argv;
	mshell = NULL;
	mshell = init_mshell(mshell, envp);
	if (!mshell)
	{
		ft_error("Fatal: Shell initialization failed.");
		return (EXIT_FAILURE);
	}
	signal_function();
	run_main_loop(mshell);
	exit_code = mshell->exit_status;
	return (exit_code);
}

/*
	>>src/bonjour <prout >> src/hello
	wc << o ls | wc << e
	wc << e | ls
	<< e
*/
