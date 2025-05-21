/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:28:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/21 18:28:56 by eduaserr         ###   ########.fr       */
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

void	ft_free_cmd(t_command **cmd)
{
	t_command	*swp;

	if ((!cmd) || (!*cmd))
		return ;
	while (*cmd)
	{
		swp = (*cmd)->next;
		if ((*cmd)->cmd)
			free((*cmd)->cmd);
		if ((*cmd)->args)
			ft_freematrix(&(*cmd)->args);
		//if ((*cmd)->redirs)
			//ft_free_redirs();
		free(*cmd);
		*cmd = swp;
	}
	*cmd = NULL;
}

void	ft_free_mshell(t_shell **mshell)
{
	if (!mshell || !*mshell)
		return ;
	if ((*mshell)->commands)
		ft_free_cmd(&(*mshell)->commands);
	if ((*mshell)->lstenv)
		ft_free_env(&(*mshell)->lstenv);
	//if ((*mshell)->p_input)
		//ft_free_str(&(*mshell)->p_input);
	if ((*mshell)->env)
		ft_freematrix(&(*mshell)->env);
	free(*mshell);
	*mshell = NULL;
}
