/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/06 02:12:09 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

//posible error, comillas anidadas ?
int	ft_check_quotes(t_shell **mshell, char *input)
{
	(void)input;
	(void)mshell;
	char	quote_type;
	int	i;

	i = 0;
	if (!input)
		return (NO_QUOTES);
	while (input[i])
	{
		if (ft_strchr(input, '\'') || ft_strchr(input, '\"'))
		{
			quote_type = input[i++];
			while (input[i] && input[i] != quote_type)
				i++;
			if (!input[i])
				return (UNCLOSED);
			if (i > 1 && input[1 - 1] == quote_type) // comillas vacias "" ''
				return (EMPTY);
		}
		i++;
	}
	return (NO_QUOTES);
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
	int	q_state;

	q_state = 0;
	q_state = ft_check_quotes(mshell, input);
	if (q_state == -1)
		return (ft_error, ft_freemshell());
	if (q_state == -2)
		remove_quotes();
	//^ check_input ^ before split into struct
	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;

	input = NULL;
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
	mshell = init_mshell(mshell, envp);
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (1)
	{
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0') // Si no es ENTER
			parse_input(&mshell, input);
		free(input);
		ft_printmatrix(mshell->user_input);
		ft_freematrix(mshell->user_input);
	}
	//ft_free_mshell(&mshell);
	return (0);
}



/*
	esto nos permite poder hacer malloc fuera del while y liberar solo cuando sea necesario :

- exit_status :
	ft_exit(int , mshell) con condiciones para liberar distintos elementos de mshell.

- valgrind errors = al ejecutar minishell, printear un input (a) y sea el siguiente input ENTER
(probablemente se arregle al avanzar el proyecto)
	*/