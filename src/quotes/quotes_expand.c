/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/22 21:39:45 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*is_var(char *str, t_env *env)
{
	if (!str)
		return (NULL);
	if (!ft_getenv(env, str))
		return (ft_error("no env key match"), NULL);
	return (ft_getenv(env, str));// devuelve puntero a estructura. //strdup() ?
}

char	*expand_var(t_shell **mshell, char **input, int i)
{
	char	*tmp;
	char	*value;
	char	*sub;
	int		end;

	(void)sub;
	(void)value;
	(void)tmp;
	end = i + 1;
	sub = NULL;
	tmp = NULL;
	value = NULL;
	while ((*input)[end] && (*input)[end] != '\"') // end = posicion de la segunda doble comilla
		end++;
	
	ft_printf("	indx expand %d\n", i);
/*	tmp = get_in_quotes(*input, i, end); // recojo "$"
	if (!tmp)
		return (NULL);
	ft_printf("TMP\n");
	ft_printlines(tmp);*/
	value = is_var(tmp, (*mshell)->lstenv);
	if (!value)
	{
		/*sub = ft_substr(*input, 0, i);
		free(*input);
		*input = NULL;
		*input = ft_strjoin(sub, "");
		return (*input);*/
	}
	/*sub = ft_substr(*input, 0, i);
	free(*input);
	*input = NULL;
	*input = ft_strjoin(sub, value);
	ft_printf("posjoin input -> %s\n", *input);*/
	return (*input);
}

//cambiar partes comentadas, 50 - 54 y 56 - 60.