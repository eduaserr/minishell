/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:28:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:14:28 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	ft_free_env(t_env **lstenv)
{
	t_env	*swp;

	swp = NULL;
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
void	ft_free_tkn(t_token **tkn)
{
	t_token	*swp;

	swp = NULL;
	if ((!tkn) || (!*tkn))
		return ;
	while (*tkn)
	{
		swp = (*tkn)->next;
		if ((*tkn)->value)
			free((*tkn)->value);
		free(*tkn);
		*tkn = swp;
	}
	*tkn = NULL;
}

void	ft_free_cmd(t_command **cmd)
{
	t_command	*swp;

	swp = NULL;
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

void	ft_free_mshell(t_shell **mshell) // free_all
{
	if (!mshell || !*mshell)
		return ;
	if ((*mshell)->tkn)
		ft_free_tkn(&(*mshell)->tkn);
	if ((*mshell)->commands)
		ft_free_cmd(&(*mshell)->commands);
	if ((*mshell)->lstenv && (!(*mshell)->running))
		ft_free_env(&(*mshell)->lstenv);
	if ((*mshell)->env && (!(*mshell)->running))
		ft_freematrix(&(*mshell)->env);
	if ((*mshell)->p_input)
		(*mshell)->p_input = ft_free_str(&(*mshell)->p_input);
	if ((!(*mshell)->running))
	{
		free(*mshell);
		*mshell = NULL;
	}
}
