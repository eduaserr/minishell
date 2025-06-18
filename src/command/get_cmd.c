/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 19:47:38 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/18 20:44:21 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	pipelen(t_token *tkn)
{
	int count;

	count = 0;
	while (tkn && tkn->value[0] != '|')
	{
		count++;
		tkn = tkn->next;
	}
	return (count);
}

void	get_args(t_token *tkn, t_command *cmd)
{
	int			i;
	int			len;

	while (cmd)
	{
		i = 0;
		len = pipelen(tkn);
		cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
		if (!cmd->args)
			return (ft_error("malloc cmd->args"));
		while (tkn && tkn->value[0] != '|' && i < len)
		{
			cmd->args[i] = ft_strdup(tkn->value);
			if (!cmd->args[i])
				return (ft_freematrix(&cmd->args), ft_error("strdup"));
			i++;
			tkn = tkn->next;
		}
		cmd->args[i] = NULL;
		if (tkn && tkn->value[0] == '|')
			tkn = tkn->next;
		cmd = cmd->next;
	}
}

static int	arrlen(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

void	swp_value(char **input, char *value, int i, int end)
{
	char	*s1;
	char	*s2;

	s1 = NULL;
	s2 = NULL;
	s1 = ft_substr(*input, 0, i);
	s2 = ft_substr(*input, end + 1, ft_strlen(*input));
	ft_free_str(input);
	*input = ft_strjoin(s1, value);
	free(s1);
	*input = ft_strjoin_gnl(*input, s2);
	free(s2);
}

void	toggle_quotes(char *str, int i, int *single, int *in_db)
{
	if (str[i] == '\'' && !*in_db)
		*single = !*single;
	else if (str[i] == '"' && !*single)
		*in_db = !*in_db;
}


static int	get_var_end(char *str, int start)
{
	int i = start;

	if (str[i] != '$')
		return (start);
	i++;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (start);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i - 1);
}


void	dollar_case(t_env *env, int e_status, char **str)
{
	char	*pid;
	char	*err;
	char	*value;
	char	*word;
	int		in_db;
	int		in_single;
	int		i;
	int		end;

	i = 0;
	in_db = 0;
	in_single = 0;
	while ((*str)[i])
	{
		toggle_quotes(*str, i, &in_single, &in_db);
		if ((*str)[i] == '$' && (*str)[i + 1] == '$' && !in_single)
		{
			pid = ft_itoa(getpid());
			swp_value(str, pid, i, i + 1);
			i += ft_strlen(pid) - 2;
			ft_free_str(&pid);
		}
		else if ((*str)[i] == '$' && (*str)[i + 1] == '?' && !in_single)
		{
			err = ft_itoa(e_status);
			swp_value(str, err, i, i + 1);
			i += ft_strlen(err) - 2;
			ft_free_str(&err);
		}
		else if ((*str)[i] == '$' && !in_single &&
			((*str)[i + 1] == '\'' || (*str)[i + 1] == '"'))
		{
			swp_value(str, "", i, i);
			i--;
		}
		else if ((*str)[i] == '$' && !in_single)
		{
			end = get_var_end(*str, i);
			// ✅ Verificar si es variable válida
			if (end > i)  // Si end > i, encontramos una variable válida
			{
				word = get_word(*str, i);  // Extraer nombre de variable
				if (word)
				{
					ft_printf("word is -> %s\n", word);
					value = ft_getenv(env, word);
					if (!value)
						value = ft_strdup("");  // Variable no existe → string vacío

					swp_value(str, value, i, end);
					i += ft_strlen(value) - 1;

					ft_free_str(&word);
					ft_free_str(&value);
				}
			}

			/*end = 0;
			if (!ft_isalpha(str[i]) && str[i] != '_')
			{
				end = i;  // Variable inválida
			}
			while ((*str)[end] && (ft_isalnum((*str)[end]) || (*str)[end] == '_'))
				end++;
			end = end - 1;
			word = get_word_msh(*str, i);		//get_word_mshell gt_var
			if (word)
			{
				ft_printf("word is -> %s\n", word);
				if (!is_var(word, env))
					value = ft_strdup("");
				else
					value = ft_getenv(env, word);
				ft_free_str(&word);
				swp_value(str, value, i, end);
				i += ft_strlen(value) - 1;
				ft_free_str(&value);
			}*/
		}
		i++;
	}
}
//delimitadores
//Válidos: a-z, A-Z, 0-9, _
// Números (NO primer carácter)
//Inválidos: espacios, !, @, #, $, %, ^, &, *, (, ), -, +, =, [, ], {, }, |, \, :, ;, ", ', <, >, ,, ., ?, /

/*
saltar el $
verificar primer caracter (letra o _)
continuar mientras ft_isalnum o _
parar en delimitador
retornmar ultima posición válida
if (end == start), variable inválida
*/
static char	*process_str(t_shell *mshell, char *str)
{
	char	*result;

	if (ft_strchr(str, '$'))
	{
		dollar_case(mshell->lstenv, mshell->last_exit_status, &str);
		result = str;
	}
	if (get_quote(str) != 0)
		result = rm_quotes2(str);
	else
		result = str;
	return (result);
}

static char	**duparr(t_shell *mshell, char **arr)
{
	char	**tmp;
	char	*str;
	int		i;

	tmp = (char **)malloc(sizeof(char *) * (arrlen(arr) + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		str = ft_strdup(arr[i]);
		
		tmp[i] = process_str(mshell, str);
		if (!tmp[i])
			return (ft_freematrix(&tmp), NULL);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	dup_cmd(t_shell *mshell, t_command *cmd)
{
	char	**arr;

	arr = NULL;
	while (cmd)
	{
		arr = duparr(mshell, cmd->args);
		ft_freematrix(&cmd->args);
		cmd->args = arr;
		cmd = cmd->next;
	}
}
