/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/01 18:39:11 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"
#include <stdlib.h>

void	cleanup_readline(void)
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

//You must indicate the key and it returns the corresponding value"
/*static char	*ft_getenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, 4) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (ft_strdup("\0"));
	return (ft_strdup(env->value));
}*/

void	promp_input(t_shell *mshell)
{
	char	*promp;

	promp = NULL;
	(void)mshell;
	//promp = getenv(mshell->lstenv, "USER");
	//promp = ft_strjoin_gnl(promp, "@mshell> ");
	promp = readline("@mshell> ");
	if (promp && promp[0] != '\0')
	{
		ft_printf("%s\n", promp);
		add_history(promp);
	}
	else // este else solo se activa cuando se presiona Ctrl + D EOF
	{
		ft_printf("exit\n");
		cleanup_readline();
		ft_free_mshell(&mshell);
		exit(EXIT_SUCCESS);
	}
/*	char *pathvar = NULL;
	pathvar = getenv("USER");
	ft_printf("pathvar=%s", pathvar);
	free(pathvar);
*/
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;

	(void)argc;
	(void)argv;
	mshell = NULL;
	signal_function();
	mshell = init_mshell(mshell, envp); //init lstenv as stack
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (1)
	{
		promp_input(mshell);//Ctrl + D signal se maneja con readline EOF
	}
	//cleanup_readline();
	//ft_free_mshell(&mshell);
	printf("minishell :)\n");
	return (0);
}
