/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 20:27:29 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/27 20:53:17 by eduaserr         ###   ########.fr       */
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

void	sync_env_array(t_shell *shell)
{
    t_env	*current;
    char	*env_string;
    int		count;
    int		i;

    // ✅ Contar nodos
    count = 0;
    current = shell->lstenv;
    while (current)
    {
        count++;
        current = current->next;
    }
    
    // ✅ Liberar array anterior
    if (shell->env)
        ft_freematrix(&shell->env);
    
    // ✅ Crear nuevo array
    shell->env = malloc(sizeof(char *) * (count + 1));
    if (!shell->env)
        return;
    
    // ✅ Convertir lista a array
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
