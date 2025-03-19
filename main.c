/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/19 22:22:55 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

t_env	*init_env(t_env *env, char **envp)
{
	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->env = ft_init_array(envp);
	//env->path = ft_init_array(env->path);
	if (!env->env || !env->path)
		return (NULL);
	return (env);
}

t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->env = init_env(mshell->env, envp);
	//mshell->user_input = ft_init_array(mshell->user_input);
	return (mshell);
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
	printf("minishell :)\n");
	return (0);
}
