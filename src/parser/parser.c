/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:25:14 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/16 03:31:34 by eduaserr         ###   ########.fr       */
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

char **ft_ndtoarr(t_token *src, int loop)
{
	char		**arr;
	char		*str;
	t_token		*swp;
	int			i;
	int			q;

	(void)q;
	q = 0;
	i = ft_nodelen(src);
	str = NULL;
	arr = NULL;
	arr = (char **)malloc(sizeof(char *) * (i + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (src && loop)
	{
		swp = src->next;
		str = ft_strdup(src->value);
		if (!str)
			return (NULL);
		q = get_quote(str);						 // de aqui
		if (q != 0)
			arr[i] = rm_quotes2(str);
		else
		{
			arr[i] = ft_strdup(str);	//copia todo
			ft_free_str(&str);			//copia todo
		}										// hasta aqui es rm_quotes
		if (!arr[i])
			return (ft_freematrix(&arr), NULL);
		i++;
		loop--;
		src = swp;
	}
	arr[i] = NULL;
	return (arr);
}


/*int		flen(t_token *tkn)
{
	int	i;

	i = 0;
	while (tkn && tkn->value[0] != '|')
	{
		tkn = tkn->next;
		i++;
	}
	return (i);
}*/

// Función auxiliar que no modifica el puntero
int pipelen(t_token *tkn)
{
    int count = 0;
    
    while (tkn && tkn->value[0] != '|')
    {
        count++;
        tkn = tkn->next;
    }
    return (count);
}

void get_args(t_token *tkn, t_command *cmd)
{
	t_token		*tmp;
	int			i;
	int			len;

	while (cmd)
	{
		i = 0;
		tmp = tkn;
		len = pipelen(tmp);
		cmd->args = (char **)malloc(sizeof(char *) * (len + 1));
		if (!cmd->args)
			return (ft_error("malloc cmd->args"));
		tkn = tmp;
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

char	**duparr(char **arr)
{
	char	**tmp;
	char	*str;
	int		i;
	int		q;

	i = 0;
	if (!arr || !*arr)
		return (NULL);
	while (arr && arr[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = 0;
	while (arr[i])
	{
		str = ft_strdup(arr[i]);
		q = get_quote(str);						 // de aqui
		if (q != 0)
			tmp[i] = rm_quotes2(str);
		else
		{
			tmp[i] = ft_strdup(str);	//copia todo
			ft_free_str(&str);			//copia todo
		}
		if (!tmp[i])
			return (ft_freematrix(&tmp), NULL);
		i++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void aux(t_command *cmd)
{
	char		**arr;

	arr = NULL;
	while (cmd)
	{
		arr = duparr(cmd->args);
		ft_freematrix(&cmd->args);
		cmd->args = arr;
		cmd = cmd->next;
	}
}

/*void	function(t_shell **mshell)
{
	t_token	 *tkn;
	t_command *cmd;
	int		i;

	i = 0;
	tkn = (*mshell)->tkn;
	cmd = (*mshell)->commands;
	while (cmd)
	{
		i = 0;
		ft_printf("flen is -> %i\n", flen(tkn));
		cmd->args = (char **)malloc(sizeof(char *) * (flen(tkn) + 1));
		if (!cmd->args)
			return (ft_error("cmd_args"));
		ft_printtkn(tkn);
		while (tkn && tkn->value && tkn->value[0] != '|' && i <= flen(tkn))
		{
			//cmd->args = ft_ndtoarr((*mshell)->tkn, i);
			ft_printf("source -> %s\n", tkn->value);
			cmd->args[i] = ft_strdup(tkn->value);
			if (!cmd->args[i])
				return (ft_error("cmd_args str"));
			ft_printf("args -> %s\n", cmd->args[i]);
			ft_printf("	2 iter = %i\n", i);
			i++;
			tkn = tkn->next;
			ft_printf("next value is -> %s\n", tkn->value);
		}
		ft_printf("	3 iter = %i\n", i);
		cmd->args[i] = NULL;
		ft_printmatrix(cmd->args);
		cmd = cmd->next;
	}
	ft_printf("	final iter = %i\n", i);
}*/

void	parse_input(t_shell **mshell, char *input)
{
	(*mshell)->p_input = preparate_input(input);
	if (!(*mshell)->p_input)
		return ;
	//parse_tokens
	(*mshell)->tkn = tokenizer((*mshell)->tkn, (*mshell)->p_input);
	if (!(*mshell)->tkn)
		return (ft_error("token"));
	if (handle_pipes_err((*mshell)->p_input, 0)) // handle_reddir
		return (ft_error("syntax error near unexpected token `|'"));
	(*mshell)->commands = get_command(*mshell, (*mshell)->commands, (*mshell)->p_input);
	if (!(*mshell)->commands)
		return (ft_error("get cmd"));
	get_args((*mshell)->tkn, (*mshell)->commands);
	if (!(*mshell)->commands->args)
		return (ft_error("get cmd args"));
	aux((*mshell)->commands);
	// parse_redirecciones
	// si es tkn->type DOUBLE ("") checkear por expansión
	// si hay $ checkear siguiente posicion para expandir
	// si es variable del sistema , sustituir
		//^ check_input ^ before split into struct
}


/*Necesitas implementar la expansión antes de eliminar las comillas.
Actualmente tienes expand_var en quotes_expand.c pero no se está usando.
*/