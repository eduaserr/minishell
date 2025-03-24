/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:28:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/24 17:44:48 by eduaserr         ###   ########.fr       */
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
	if (!mshell)
		return ;
	if (mshell->lstenv)
		ft_free_env(mshell->lstenv);
	if (mshell->user_input)
		ft_freemap(mshell->user_input);
	if (mshell->env)
		ft_freemap(mshell->env);
	free(mshell);
}
