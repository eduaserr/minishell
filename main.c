/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:31:14 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	//update_struct();
	if ((*mshell)->exit_status)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	char	*input;

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
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0') // Si no es ENTER
			parse_input(&mshell, ft_strdup(input));
		ft_printf("main input -> %s\n", input);
		input = ft_free_str(&input);
		//ft_printenv(mshell->lstenv);
		/* ft_printcmd(mshell->commands); */
		ft_printtkn(mshell->tkn);
		ft_printf("process input -> %s\n", mshell->p_input);
		update_shell(&mshell);
		//updateshlvl();
	}
	return (0);
}
