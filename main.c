/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/27 20:12:26 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	// update struct() ?
	if ((*mshell)->exit_status == 1)
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
		input = promp_input(mshell);
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0')
			add_history(input);
		if (input[0] == '\0' || ft_isspace(input[0]))
		{
			free(input);
			continue ;
		}
		mshell->user_input = input;
		parse_input(&mshell, ft_strdup(input));
		if (mshell->commands)
		{
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
		free(input);
	}
	return (0);
}
