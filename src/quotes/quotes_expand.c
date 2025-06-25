/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/25 22:59:51 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	toggle_quotes(char *str, int i, int *single, int *in_db)
{
	if (str[i] == '\'' && !*in_db)
		*single = !*single;
	else if (str[i] == '"' && !*single)
		*in_db = !*in_db;
}

static int	dollar_d(char **str, int i)
{
	char	*pid;

	pid = ft_itoa(getpid());
	if (!pid)
		pid = ft_strdup("");
	swp_value(str, pid, i, i + 1);
	i += ft_strlen(pid) - 2;
	ft_free_str(&pid);
	return (i);
}

static int	dollar_qu(char **str, int e_status, int i)
{
	char	*err;

	err = ft_itoa(e_status);
	if (!err)
		ft_strdup("");
	swp_value(str, err, i, i + 1);
	i += ft_strlen(err) - 2;
	ft_free_str(&err);
	return (i);
}

void	dollar_case(t_env *env, int e_status, char **str)
{
	int		in_db;
	int		in_single;
	int		i;

	i = 0;
	in_db = 0;
	in_single = 0;
	while ((*str)[i])
	{
		toggle_quotes(*str, i, &in_single, &in_db);
		if ((*str)[i] == '$' && (*str)[i + 1] == '$' && !in_single)
			i = dollar_d(str, i);
		else if ((*str)[i] == '$' && (*str)[i + 1] == '?' && !in_single)
			i = dollar_qu(str, e_status, i);
		else if ((*str)[i] == '$' && !in_single &&
			((*str)[i + 1] == '\'' || (*str)[i + 1] == '"'))
		{
			swp_value(str, "", i, i);
			i--;
		}
		else if ((*str)[i] == '$' && !in_single)
			i = dollar_expand(str, env, i);
		i++;
	}
}
