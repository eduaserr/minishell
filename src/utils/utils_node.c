/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:27:29 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/18 19:31:28 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//Creates strdup. You must indicate the key and it returns the corresponding value"
char	*ft_getenv(t_env *env, char *var)
{
	while (env)
	{
		if (ft_strncmp(env->key, var, ft_strlen(var)) == 0)
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
	return (value);// devuelve puntero a estructura. //strdup() ?
}
