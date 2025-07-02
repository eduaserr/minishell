/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:27:29 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/02 18:32:54 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*ft_getlstenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, ft_strlen(var)) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (NULL);
	return (env);
}

char	*ft_getenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, ft_strlen(env->key)) == 0)
			break ;
		env = env->next;
	}
	if (!env)
		return (NULL);
	return (ft_strdup(env->value));
}

char	*is_var(char *str, t_env *env)
{
	char	*value;

	if (!str)
		return (NULL);
	value = ft_getenv(env, str);
	if (!value)
		return (ft_error("no env key match"), NULL);
	return (value);
}

/* void	sync_cpy(t_shell *shell, t_env *current, int i, int count)
{
	char	*tmp;

	tmp = NULL;
	while (current && i < count)
	{
		tmp = ft_strjoin(current->key, "=");
		if (tmp)
		{
			shell->env[i] = ft_strjoin(tmp, current->value);
			if (!shell->env[i])
				return ();
			ft_free_str(&tmp);
		}
		current = current->next;
		i++;
	}
	shell->env[i] = NULL;
}

void	sync_env_array(t_shell *shell)
{
	t_env	*current;
	int		count;
	int		i;

	count = 0;
	current = shell->lstenv;
	while (current)
	{
		count++;
		current = current->next;
	}
	if (shell->env)
		ft_freematrix(&shell->env);
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
		return ;
	i = 0;
	current = shell->lstenv;
	sync_cpy(shell, current, i, count);
} */


void	sync_env_array(t_shell *shell)
{
	t_env	*current;
	char	*env_string;
	int		count;
	int		i;

	count = 0;
	current = shell->lstenv;
	while (current)
	{
		count++;
		current = current->next;
	}
	if (shell->env)
		ft_freematrix(&shell->env);
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
		return ;
	i = 0;
	current = shell->lstenv;
	while (current && i < count)
	{
		env_string = ft_strjoin(current->key, "=");
		if (env_string)
		{
			shell->env[i] = ft_strjoin(env_string, current->value);
			free(env_string);
		}
		current = current->next;
		i++;
	}
	shell->env[i] = NULL;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
