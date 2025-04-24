/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/24 20:49:30 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

char	**ft_split_input(char *str)
{
	char	**input;

	input = ft_split(str, ' ');
	if (!input || !*input)
		return (NULL);
	return (input);
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;

	input = NULL;
	promp = NULL;
	//getcwd();
	promp = ft_getenv(mshell->lstenv, "USER");
	promp = ft_strjoin_gnl(promp, "@mshell> ");
	input = readline(promp);
	ft_free_str(&promp);
	if (!input)
		return (NULL);
	if (input && input[0] != '\0')
		add_history(input);
	return (input);
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
	while (1)
	{
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0') // Si no es ENTER
			parse_input(&mshell, ft_strdup(input));
		//ft_printlines(input);
		free(input);
		//ft_freematrix(mshell->user_input);
	}
	//ft_free_mshell(&mshell);
	return (0);
}



/*
	exit_status : esto nos permite poder hacer malloc fuera del while(1)
	y liberar solo cuando sea necesario :

- exit_status :
	ft_exit(int , mshell) con condiciones para liberar distintos elementos de mshell.

- valgrind errors = al ejecutar minishell, printear un input (a) y sea el siguiente input ENTER
(probablemente se arregle al avanzar el proyecto)
	*/