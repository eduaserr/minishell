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

static void	execute_command_pipeline(t_shell *shell)
{
	int	pid;
	int	status;

	sync_env_array(shell);
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
		//ft_printtkn(shell->tkn);
		//ft_printcmd(shell->commands);
		if (shell->commands)
			execute_command_pipeline(shell);
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
