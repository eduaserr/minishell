/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/23 22:10:42 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	swp_value(char **input, char *value, int i, int end)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
	ft_printf("SWP___VALUE\n");
	s1 = ft_substr(*input, 0, i);
	ft_printf("		s1 -> %s\n", s1);
	s2 = ft_substr(*input, end + 1, ft_strlen(*input));
	ft_printf("		s2 -> %s\n", s2);
	free(*input);
	*input = NULL;
	*input = ft_strjoin(s1, value);
	free(s1);
	*input = ft_strjoin_gnl(*input, s2);
	free(s2);
}

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
	tmp = get_in_quotes(*input, i, end); // recojo "$"
	if (!tmp)
		return (NULL);
	ft_printf("TMP\n");
	ft_printlines(tmp);
	value = is_var(tmp, (*mshell)->lstenv);
	ft_printf("value is -> %s\n", value);
	if (!value)
	{
		*input = rm_quotes(input, i);
		return (*input);
	}
	swp_value(input, value, i, end);
	ft_printf("posjoin input -> %s\n", *input);
	return (*input);
}
