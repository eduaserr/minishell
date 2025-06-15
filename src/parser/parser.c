/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/15 15:08:55 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_pipes_err(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '|' && i == 0)
			return (1);
		if (str[i] == '|' && str[i + 1] == '|')
			return (1);
		i++;
	}
		// esto lo puedo comprobar en la linea guardada del comando
	if (i > 0 && str[i - 1] == '|')// Check for pipe at the end
		return (1);
	return (0);
}

static char	*preparate_input(char *input)
{
	char	*tmp;

	tmp = ft_strtrim(input, " \t\n\r\v\f");
	ft_free_str(&input);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')		//intro con solo espacios
		return (ft_free_str(&tmp));
	tmp = check_quotes(tmp);
	if (!tmp)
		return (NULL);
	if (tmp[0] == '\0')
		return (ft_free_str(&tmp));
	input = ft_strtrim(tmp, " \t\n\r\v\f");
	ft_free_str(&tmp);
	if (!input)
		return (NULL);
	if (input[0] == '\0')
		return (ft_free_str(&input));
	return (input);
}

char *rm_quote_chars(char *str, int start_quote, int end_quote)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	len = ft_strlen(str);
	i = 0; 
	j = 0;
	result = (char *)malloc(sizeof(char) * (len - 1));  // -1 porque eliminamos 2 caracteres
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (i != start_quote && i != end_quote)  // Salta las posiciones de las comillas
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

char *rm_quotes2(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp = NULL;
	while (str[i])
	{
		ft_printf("while %i\n", i);
		j = i;
		if (skip_quoted(str, &i))
		{
			ft_printf("ENTRA SKIP_QUOTED\n");
			ft_printf("	first_quotes = %i\n", j);
			ft_printf("	str[%i] -> %c\n", j, str[j]);
			ft_printf("	last_quotes = %i\n", i - 1);
			ft_printf("	str[%i] -> %c\n", i - 1, str[i - 1]);
			tmp = rm_quotes(str, j, i - 1);
                if (!tmp)
                    return (NULL);
			ft_printf("	-string str before -> %s\n", tmp);
			ft_printf("	-string tmp after -> %s\n", tmp);
			ft_free_str(&str);
			str = tmp;
			ft_printf("	-string str after -> %s\n", tmp);
		}
		else
		{
			ft_printf("NO SKIP_QUOTED\n");
			i++;
		}
		
	}
	return (tmp);
}

int		ft_nodelen(t_token *tkn)
{
	int		i;

	i = 0;
	while (tkn)
	{
		i++;
		tkn = tkn->next;
	}
	return (i);
}

char **ft_ndtoarr(t_token *src)
{
	char		**arr;
	char		*str;
	t_token		*swp;
	int			i;
	int			q;

	q = 0;
	i = ft_nodelen(src);
	str = NULL;
	arr = NULL;
	arr = (char **)malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (src)
	{
		swp = src->next;
		ft_printf("src->value %s\n", (src->value));
		str = ft_strdup(src->value);
		if (!str)
			return (NULL);
		ft_printf("strdup is %s\n", (str));
		q = get_quote(str);
		if (q != 0)
		{
			arr[i] = rm_quotes2(str);
			ft_printf("	array[%i] quotes %s\n", i, (arr[i]));
		}
		else
		{
			arr[i] = ft_strdup(str);
			ft_printf("	array[%i] no quotes %s\n", i, (arr[i]));
		}
		ft_free_str(&str);
		if (!arr[i])
			return (ft_freematrix(&arr), NULL);
		ft_printf("array[%i] al final %s\n", i, (arr[i]));
		i++;
		src = swp;
	}
	ft_printmatrix(arr);
	arr[i] = NULL;
	return (arr);
}

void	parse_input(t_shell **mshell, char *input)
{
	(*mshell)->p_input = preparate_input(input);
	if (!(*mshell)->p_input)
		return ;
	//parse_tokens
	(*mshell)->tkn = tokenizer((*mshell)->tkn, (*mshell)->p_input);
	if (!(*mshell)->tkn)
		return (free((*mshell)->p_input), ft_error("token"));
	if (handle_pipes_err((*mshell)->p_input, 0)) // handle_reddir
		return (free((*mshell)->p_input), ft_error_exit(mshell, "syntax error near unexpected token `|'", 0));
	//split por comillas¿
	(*mshell)->commands = get_command(*mshell, (*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (free((*mshell)->p_input), ft_error_exit(mshell, "get command", 0));
	(*mshell)->commands->args = ft_ndtoarr((*mshell)->tkn);
	if (!(*mshell)->commands->args)
		return (ft_error("get cmd args"));
		//^ check_input ^ before split into struct
}
