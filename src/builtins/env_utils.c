/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:32:08 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/17 18:03:51 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	getenv_index(char *env, char **envp)
{
	int		i;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(env);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], env, name_len) == 0 && envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_env(char *env, char **envp)
{
	int		i;
	char	*entry;
	char	*value;
	size_t	name_len;

	i = 0;
	name_len = ft_strlen(env);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], env, name_len) == 0 && envp[i][name_len] == '=')
		{
			entry = envp[i] + name_len + 1;
			value = ft_strdup(entry);
			return (value);
		}
		i++;
	}
	return (NULL);
}

int	unset_env(char *env, char **envp)
{
	int	i;

	if (!env)
		return (0);
	i = getenv_index(env, envp);
	if (i != -1)
	{
		free(envp[i]);
		while (envp[i + 1])
		{
			envp[i] = envp[i + 1];
			i++;
		}
		envp[i] = NULL;
	}
	return (0);
}

static void	*ft_realloc(void *ptr, size_t new, size_t old)
{
	void	*new;

	if (new == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new));
	new = malloc(new);
	if (!new)
	{
		free(ptr);
		return (NULL);
	}
	ft_memcpy(new, ptr, old);
	free(ptr);
	return (new);
}

int	set_env(char *env, char ***envp)
{
	int		i;
	int		name_len;
	char	*equal_sign;

	equal_sign = ft_strchr(env, '=');
	if (!equal_sign)
		return (-1);
	name_len = equal_sign - env;
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp(env, (*envp)[i], name_len + 1))
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(env);
			return (0);
		}
		i++;
	}
	*envp = ft_realloc(*envp, (i + 2) * sizeof(char *), i * sizeof(char *));
	if (!(*envp))
		return (-1);
	(*envp)[i] = ft_strdup(env);
	(*envp)[i + 1] = NULL;
	return (0);
}
