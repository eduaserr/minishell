/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/28 14:12:11 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	promp_input(t_shell *mshell)
{
	(void)mshell;
	char	*input;

	input = readline("minishell> ");
	if (!input)
	{
		exit(EXIT_SUCCESS);
	}
	ft_printf("%s\n", input);
	input = ft_free_str(&input);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;

	(void)argc;
	(void)argv;
	mshell = NULL;
	//signal_function();
	mshell = init_mshell(mshell, envp); //init lstenv as stack
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (1)
		promp_input(mshell);//Ctrl + D signal se maneja con readline ? EOF*/
	printf("minishell :)\n");
	ft_free_mshell(mshell);
	return (0);
}
