/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:25:51 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/04 15:25:52 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_env_list(char **envp)
{
	t_env	*list;
	char	*key;
	char	*value;
	int		i;
	int		j;

	list = NULL;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=')
		{
			key = malloc(j + 1);
			if (!key)
				return (NULL);
			strncpy(key, envp[i], j);
			key[j] = '\0';
			value = ft_strdup(envp[i] + j + 1);
			if (!value)
			{
				free(key);
				return (NULL);
			}
			list = add_env_node(list, key, value);
			free(key);
			free(value);
		}
		i++;
	}
	return (list);
}

t_env	*add_env_node(t_env *list, char *key, char *value)
{
	t_env	*new_node;
	t_env	*temp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	if (!list)
		return (new_node);
	temp = list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_node;
	return (list);
}

char	*get_env_value(t_env *list, char *key)
{
	t_env	*temp;

	temp = list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

void	set_env_value(t_env *list, char *key, char *value)
{
	t_env	*temp;

	temp = list;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return ;
		}
		temp = temp->next;
	}
	add_env_node(list, key, value);
}

char	**env_list_to_array(t_env *list)
{
	t_env	*temp;
	char	**env_array;
	int		count;
	int		i;
	char	*tmp;

	count = 0;
	temp = list;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	temp = list;
	while (temp)
	{
		tmp = ft_strjoin(temp->key, "=");
		env_array[i] = ft_strjoin(tmp, temp->value);
		free(tmp);
		temp = temp->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}