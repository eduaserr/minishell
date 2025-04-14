/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:18:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/14 03:44:26 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_in_quotes(char *str, int start, int end)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	len = end - start - 1;
	if (start < 0)
		return (NULL);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	printf("trim quotes in : %s\n", str);
	while (str[++start] && (--len) >= 0)
	{
		tmp[i] = str[start];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

//Creates strdup. You must indicate the key and it returns the corresponding value"
char	*ft_getenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, 4) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (ft_strdup(""));
	return (ft_strdup(env->value));
}

t_env	*split_env(t_env *new, char *env)
{
	char	**arr;

	arr = NULL;
	arr = ft_split(env, '=');
	if (!arr || !arr[0])
		return (ft_error("split"), NULL);
	new->key = ft_strdup(arr[0]);
	if (!new->key)
		return (ft_freematrix(arr), NULL);
	if (!arr[1])
		new->value = ft_strdup("");
	else
		new->value = ft_strdup(arr[1]);
	if (!new->value)
		return (ft_freematrix(arr), NULL);
	return (ft_freematrix(arr), new);
}

char	**ft_init_array(char **array)
{
	char	**tmp;
	int		i;

	i = 0;
	if (!array)
		return (NULL);
	while (array && array[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (array[i])
	{
		tmp[i] = ft_strdup(array[i]);
		if (!tmp[i])
		{
			ft_freematrix(tmp);
			return (NULL);
		}
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	ft_printenv(t_env *lstenv)
{
	int	i;

	i = 0;
	while (lstenv)
	{
		ft_printf("node %d\n[0] - %s\n", i++, lstenv->key);
		ft_printf("[1] - %s\n", lstenv->value);
		lstenv = lstenv->next;
	}
}
