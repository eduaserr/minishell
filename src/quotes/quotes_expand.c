/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/15 22:18:58 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*is_var(char *str, t_env *env)
{
	if (!ft_getenv(env, str))
		return (NULL);
	return (ft_getenv(env, str));// devuelve puntero a estructura. //strdup() ?
}

char	*expand_var(t_shell **mshell, char **input, int i)
{
	char	*tmp;
	char	*sub;
	int		end;

	(void)sub;
	end = i + 1;
	sub = NULL;
	tmp = NULL;
	while ((*input)[end] && (*input)[end] != '\"') // segunda doble comilla
		end++;
	//if (!((*input)[i + 1] == '$')) //checkear lo que hay después de '$'
	//	return (); // no hay para expandir
	
	ft_printf("	indx expand %d\n", i);
	tmp = get_in_quotes(*input, i, end); // recojo "$"
	if (!tmp)
		return (NULL);
	if (is_var(tmp, (*mshell)->lstenv))

	sub = ft_substr(*input, 0, i); // i = primera comilla
	ft_printlines(sub);
	*input = ft_strjoin(sub, tmp);
	free(sub);
	free(tmp);

	if (ft_strchr(*input, '$'))
	{
		free(*input);
		*input = NULL;
		return (tmp);
	}
	ft_printf("return input w/ no $\n");
	return (*input);
}
