/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:47:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/17 03:55:17 by eduaserr         ###   ########.fr       */
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

void	swp_value(char **input, char *value, int i, int end)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
	ft_printf("SWP___VALUE\n	%s\n", *input);
	ft_printf("value -> %s\n", value);
	s1 = ft_substr(*input, 0, i);
	ft_printf("		s1 -> %s\n", s1);
	s2 = ft_substr(*input, end + 1, ft_strlen(*input));
	ft_printf("		s2 -> %s\n", s2);
	ft_free_str(input);
	*input = ft_strjoin(s1, value);
	ft_printf("pre input -> %s\n", *input);
	free(s1);
	*input = ft_strjoin_gnl(*input, s2);
	free(s2);
	ft_printf("pos input -> %s\n", *input);
}

static char	*process_str(t_shell *mshell, char *str, int type)
{
	char	*result;
	int		i;
	char	*pid;
	char	*err;

	i = 0;
	if (ft_strchr(str, '$') && mshell->tkn->type != SIMPLE)
	{
		ft_printf("- type -> %i\n", type);
		while (str[i])
		{
			if (str[i] == '$' && str[i + 1] == '$')
			{
				pid = ft_itoa(getpid());
				ft_printf("input -> %s\n", str);
				ft_printf("copy pid -> %s\n", pid);
				ft_printf("	i - start -> %i\n", i);
				ft_printf("	i - end -> %i\n", i + 1);
				swp_value(&str, pid, i, i + 1);
				ft_printf("copied str -> %s\n", str);
				i += ft_strlen(pid);
				ft_printf("	i - iter -> %i\n", i);
				free(pid);
			}
			else if (str[i] == '$' && str[i + 1] == '?')
			{
				err = ft_itoa(mshell->last_exit_status);
				ft_printf("input -> %s\n", str);
				ft_printf("copy err -> %s\n", err);
				ft_printf("	i - start -> %i\n", i);
				ft_printf("	i - end -> %i\n", i + 1);
				swp_value(&str, err, i, i + 1);
				ft_printf("copied str -> %s\n", str);
				i += ft_strlen(err);
				ft_printf("	i - iter -> %i\n", i);
				free(err);
			}
			ft_printf("	i - final -> %i\n", i);
			i++;
		}
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
	t_token	*tkn;
	char	**tmp;
	char	*str;
	int		i;

	tmp = (char **)malloc(sizeof(char *) * (arrlen(arr) + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	tkn = mshell->tkn;
	while (arr[i] && tkn)
	{
		str = ft_strdup(arr[i]);
		
		tmp[i] = process_str(mshell, str, tkn->type);
		if (!tmp[i])
			return (ft_freematrix(&tmp), NULL);
		i++;
		tkn = tkn->next;
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
