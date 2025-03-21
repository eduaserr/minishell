/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/21 13:14:37 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_env	*init_env(t_env *env, char **envp)
{
	env = (t_env *)malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->env = ft_init_array(envp);
	env->path = NULL;
	return (env);
}

t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->env = init_env(mshell->env, envp);
	if (!mshell->env)
		return (ft_free_mshell(mshell), NULL);
	mshell->user_input = NULL;
	return (mshell);
}
