/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/07 21:03:31 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

//esta funcion comprueba  que la primera ocurrencia de comillas tenga su respectivo cierre,
//sin importar qué contenga en su interior, incluidas comillas de distinto tipo.
// ej.: echo "hola'".  echo "a'b""c"
//		hola'
int	ft_check_quotes(t_shell **mshell, char *input, int *i)
{
	(void)input;
	(void)mshell;
	char	quote_type;

	if (!input)
		return (NO_QUOTES);
	if (ft_strchr(input, '\'') || ft_strchr(input, '\"'))
	{
		quote_type = input[*i++];
		while (input[*i] && input[*i] != quote_type)
			*i++;
		if (!input[*i])
			return (UNCLOSED);
		if (*i > 1 && input[*i - 1] == quote_type) // comillas vacias "" ''
			return (EMPTY);
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
	int	i;

	i = 0;
	q_state = 0;
	while (input[i])
	{
		q_state = ft_check_quotes(mshell, input, &i);
		if (q_state == -1)
			return (ft_error("Check_quotes"), ft_free_mshell(mshell));
		if (q_state == -2)
			printf("remove_quotes\n");
		i++;
	}
	/*^ check_input ^ before split into struct
	(*mshell)->user_input = ft_split_input(input);
	if (!(*mshell)->user_input)
		return ;*/
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
	esto nos permite poder hacer malloc fuera del while(1) y liberar solo cuando sea necesario :

- exit_status :
	ft_exit(int , mshell) con condiciones para liberar distintos elementos de mshell.

- valgrind errors = al ejecutar minishell, printear un input (a) y sea el siguiente input ENTER
(probablemente se arregle al avanzar el proyecto)
	*/