/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/21 13:30:39 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	read_user_input(t_shell *mshell)
{
	(void)mshell;
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		exit(EXIT_SUCCESS);
	}
	ft_printf("%s\n", input);
	//free input
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;

	(void)argc;
	(void)argv;
	mshell = NULL;
	signal_function();
	mshell = init_mshell(mshell, envp);
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (1)
		read_user_input(mshell);//Ctrl + D signal se maneja con readline
	printf("minishell :)\n");
	ft_free_mshell(mshell);
	return (0);
}
