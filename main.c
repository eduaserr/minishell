/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/03 18:17:04 by eduaserr         ###   ########.fr       */
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

void	parse_input(t_shell **mshell, char *input)
{
	(*mshell)->user_input = ft_split_input(input);
	//if (!(*mshell)->user_input)
	//	return ;
	//ft_check_quotes();
// free user_input
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;

	input = NULL;
	promp = NULL;
	promp = ft_getenv(mshell->lstenv, "USER");
	promp = ft_strjoin_gnl(promp, "@mshell> ");
	input = readline(promp); // readline leaks
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
	while (1)
	{
		mshell = init_mshell(mshell, envp); //init lstenv as stack
		if (!mshell)
			return (ft_error("init minishell"), 0);
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0')
			parse_input(&mshell, input);
		free(input);
		ft_printmatrix(mshell->user_input);
		ft_free_mshell(&mshell);
	}
	return (0);
}
