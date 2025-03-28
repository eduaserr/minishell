/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/28 16:22:51 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

static char	*ft_getenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, 4) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (ft_strdup(""));
	return (env->value);
}

/* #include <stdlib.h>

char *pathvar = NULL;

pathvar = getenv("USER");
ft_printf("pathvar=%s", pathvar);
free(pathvar);
 */

void	promp_input(t_shell *mshell)
{
	(void)mshell;
	char	*input;

	input = NULL;
	input = ft_getenv(mshell->lstenv, "USER");
	input = ft_strjoin(input, "@mshell> ");
	readline(input);
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
	{
		promp_input(mshell);//Ctrl + D signal se maneja con readline ? EOF
		ft_free_mshell(mshell);
	}
	printf("minishell :)\n");
	return (0);
}
