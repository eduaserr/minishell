/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:18:07 by eduaserr          #+#    #+#             */
/*   Updated: 2025/03/28 04:42:08 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*split_env(t_env *new, char *env)
{
	char	**arr;

	arr = NULL;
	arr = ft_split(env, '=');
	if (!arr || !arr[0] || !arr[1])
		return (ft_error("split"), NULL);
	//ft_printmatrix(arr);
	new->key = ft_strdup(arr[0]);
	if (!new->key)
		return (ft_freematrix(arr), NULL);
	//ft_printf("key : %s\n", new->key);
	if (arr[1])
		new->value = ft_strdup(arr[1]);
	if (!new->value)
		return (ft_freematrix(arr), NULL);
	//ft_printf("value : %s\n", new->value);
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

/*void	ft_printenv(t_env *lstenv)
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
*/