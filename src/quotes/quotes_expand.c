/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/16 01:55:00 by eduaserr         ###   ########.fr       */
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
	while ((*input)[end] && (*input)[end] != '\"') // end = posicion de la segunda doble comilla
		end++;
	
	ft_printf("	indx expand %d\n", i);
	tmp = get_in_quotes(*input, i, end); // recojo "$"
	if (!tmp)
		return (NULL);
	ft_printf("TMP\n");
	ft_printlines(tmp);
	if (is_var(tmp, (*mshell)->lstenv))
		return (ft_strdup(ft_getenv((*mshell)->lstenv, tmp)));
	sub = ft_substr(*input, 0, i); // i = primera comilla
	ft_printf("SUB\n");
	ft_printlines(sub);
	

	if (!ft_strchr(tmp, '$'))
	{
		ft_printf("no $ in input\n");
		free(*input);
		*input = NULL;
		*input = ft_strjoin(sub, tmp);
		free(sub);
		free(tmp);
		return (*input);
	}
	ft_printf("return input w/ no $\n");
	return (*input);
}
