/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/01 18:13:20 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	// update struct() ?
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	char	*input;
	int		pid;
	int		status;

	(void)argc;
	(void)argv;
	mshell = NULL;
	signal_function();
	mshell = init_mshell(mshell, envp);
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (mshell->running)
	{
		mshell->user_input = input;
		input = promp_input(mshell);
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0')
		{
			parse_input(&mshell, ft_strdup(input));
			ft_printcmd(mshell->commands);
			if (mshell->commands)
			{
				sync_env_array(mshell);
				if (execute_parent_builtin(mshell->commands->args, mshell))
				{
					update_shell(&mshell);
					free(input);
					continue ;
				}
				pid = fork();
				if (pid == 0)
					execute(mshell, mshell->commands->args, mshell->env);
				waitpid(pid, &status, 0);
			}
			update_shell(&mshell);
		}
		free(input);
	}
	return (0);
}
