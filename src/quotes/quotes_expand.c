/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:35:08 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/17 00:12:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/*
static void	swp_value(char **input, char *value, int i, int end)
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
}*/

// if $USER se deberia comprobar en todo el input, no solo en el strign entre comillas. ?
/*char	*expand_var(t_shell **mshell, char **input, char **args)
{
	t_command	*node;
	int	i;

	node = (*mshell)->commands;
	i = 0;
	while (node && args)
	{
		while (args[i])
		{
			if (args[0] == '$' && (*mshell)->tkn->type != SIMPLE)
			{
				if (args[1] == '$')
				if (ft_isalpha(args[1]) || args[1] == '_')
				
			}
		}
	}


	char	*tmp;
	char	*value;
	int		end;

	(void)value;
	(void)tmp;
	end = i + 1;
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
	tmp "USER" || "hola USER"
	antes de usar tmp, comprobar que :
	- USER no contenga caracteres alfanumericos detrás (numero y letras).
	ej.: "hola USER" || "hola USER, bienvenido"


	value = is_var(tmp, (*mshell)->lstenv); // devuelve NULL si no hay coincidencia despues del dollar $
	ft_printf("value is -> %s\n", value);
	tmp = ft_free_str(&tmp);
	if (!value)
	{
		tmp = rm_quotes(*input, i, 0);
		if (tmp)
		{
			*input = ft_free_str(input);
			*input = tmp;
		}
		return (*input);
	}
	swp_value(input, value, i, end);
	value = ft_free_str(&value);
	ft_printf("posjoin input -> %s\n", *input);
	return (*input);
}
*/