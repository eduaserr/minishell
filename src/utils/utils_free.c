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

void	ft_free_env(t_env *lstenv)
{
	t_env	*stack;

	while (lstenv)
	{
		stack = lstenv->next;
		if (lstenv->key)
			free(lstenv->key);
		if (lstenv->value)
			free(lstenv->value);
		free(lstenv);
		lstenv = stack;
	}
}

void	ft_free_mshell(t_shell *mshell)
{
	if (mshell->lstenv)
		ft_free_env(mshell->lstenv);
	if (mshell->user_input)
		ft_freemap(mshell->user_input);
	if (mshell->env)
		ft_freemap(mshell->env);
	if (mshell)
		free(mshell);
}
