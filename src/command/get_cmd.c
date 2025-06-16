/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:47:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 20:16:20 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	pipelen(t_token *tkn)
{
	int count;

	count = 0;
	while (tkn && tkn->value[0] != '|')
	{
		count++;
		tkn = tkn->next;
	}
	return (count);
}

void	get_args(t_token *tkn, t_command *cmd)
{
	int			i;
	int			len;

	while (cmd)
	{
		i = 0;
		len = pipelen(tkn);
		cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
		if (!cmd->args)
			return (ft_error("malloc cmd->args"));
		while (tkn && tkn->value[0] != '|' && i < len)
		{
			cmd->args[i] = ft_strdup(tkn->value);
			if (!cmd->args[i])
				return (ft_freematrix(&cmd->args), ft_error("strdup"));
			i++;
			tkn = tkn->next;
		}
		cmd->args[i] = NULL;
		if (tkn && tkn->value[0] == '|')
			tkn = tkn->next;
		cmd = cmd->next;
	}
}

static int	arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static char	**duparr(char **arr)
{
	char	**tmp;
	char	*str;
	int		i;

	tmp = (char **)malloc(sizeof(char *) * (arrlen(arr) + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		str = ft_strdup(arr[i]);
		if (get_quote(str) != 0)
			tmp[i] = rm_quotes2(str);
		else
		{
			tmp[i] = ft_strdup(str);
			ft_free_str(&str);
		}
		if (!tmp[i])
			return (ft_freematrix(&tmp), NULL);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	dup_cmd(t_command *cmd)
{
	char	**arr;

	arr = NULL;
	while (cmd)
	{
		arr = duparr(cmd->args);
		ft_freematrix(&cmd->args);
		cmd->args = arr;
		cmd = cmd->next;
	}
}
