/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/01 22:25:46 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	ft_clean_rl(void)
{
    // Limpia el historial de readline
    rl_clear_history();

    // Libera el estado de la línea actual
    rl_free_line_state();

    // Limpia la configuración de la terminal
    rl_deprep_terminal();

    // Limpia recursos internos de readline
    rl_cleanup_after_signal();
}


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

	promp = NULL;
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
	mshell = init_mshell(mshell, envp); //init lstenv as stack
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (1)
	{
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		parse_input(&mshell, input);
		free(input);
		//ft_printmatrix(mshell->user_input);
		
	}
	return (0);
}
