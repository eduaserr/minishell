/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:28:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/20 17:39:27 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free_env(t_env *env)
{
	if (env->env)
		ft_freemap(env->env);
	if (env->path)
		ft_freemap(env->path);
	if (env)
		free(env);
}

void	ft_free_mshell(t_shell *mshell)
{
	if (mshell->env)
		ft_free_env(mshell->env);
	if (mshell->user_input)
		ft_freemap(mshell->user_input);
	if (mshell)
		free(mshell);
}
