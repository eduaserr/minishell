/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:47:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/19 00:10:45 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	pipelen(t_token *tkn)
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

static int	arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

static char	*process_str(t_shell *mshell, char *str)
{
	char	*result;

	if (ft_strchr(str, '$'))
	{
		dollar_case(mshell->lstenv, mshell->last_exit_status, &str);
		result = str;
	}
	if (get_quote(str) != 0)
		result = rm_quotes2(str);
	else
		result = str;
	return (result);
}

static char	**duparr(t_shell *mshell, char **arr)
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
		
		tmp[i] = process_str(mshell, str);
		if (!tmp[i])
			return (ft_freematrix(&tmp), NULL);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	dup_cmd(t_shell *mshell, t_command *cmd)
{
	char	**arr;

	arr = NULL;
	while (cmd)
	{
		arr = duparr(mshell, cmd->args);
		ft_freematrix(&cmd->args);
		cmd->args = arr;
		cmd = cmd->next;
	}
}
