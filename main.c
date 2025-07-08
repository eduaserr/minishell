/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/08 21:41:43 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	(*mshell)->cmd_count = 0;
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

static void	remove_heredocs(t_shell *shell)
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

		return (ft_error("minishell: fork"));
	if (pid == 0)
		execute(shell, shell->commands->args, shell->env);
	else
	{
		waitpid(pid, &status, 0);
		clean_sigint(shell, status);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
}

static void	process_command_line(t_shell *shell)
{
	char	*input;

	while (shell->running)
	{
		input = promp_input(shell);
		if (!input)
			ft_exit(&shell);
		if (g_signal_received == SIGINT)
		{
			free(input);
			continue ;
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
	process_command_line(mshell);
	exit_code = mshell->exit_status;
	return (exit_code);
}
