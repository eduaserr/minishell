/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:28:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/01 17:56:57 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	ft_free_env(t_env **lstenv)
{
	t_env	*swp;

	if ((!lstenv) || (!*lstenv))
		return ;
	while (*lstenv)
	{
		swp = (*lstenv)->next;
		if ((*lstenv)->key)
			free((*lstenv)->key);
		if ((*lstenv)->value)
			free((*lstenv)->value);
		free(*lstenv);
		*lstenv = swp;
	}
	*lstenv = NULL;
}

void	ft_free_mshell(t_shell **mshell)
{
	if (!mshell || !*mshell)
		return ;
	if ((*mshell)->lstenv)
		ft_free_env(&(*mshell)->lstenv);
	if ((*mshell)->user_input)
		ft_freematrix((*mshell)->user_input);
	if ((*mshell)->env)
		ft_freematrix((*mshell)->env);
	free(*mshell);
	*mshell = NULL;
}
