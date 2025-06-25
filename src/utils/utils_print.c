/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:56:18 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/25 23:08:57 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_printtkn(t_token *tkn)
{
	int	i;

	i = 0;
	ft_printf("PRINT TOKENS\n\n");
	while (tkn)
	{
		ft_printf("node %d\n	token->value - %s\n", i++, tkn->value);
		ft_printf("	int type - %i\n", tkn->type);
		tkn = tkn->next;
	}
}

static void	ft_printrd(t_redir *rd, int ix)
{
	int	i;

	i = 0;
	if (rd)
	{
		while (rd)
		{
			ft_printf("PRINT REDIR %i\n", ix);
			ft_printf("node %d\ntype - %i\n", i++, rd->type);
			ft_printf("file is -> %s\n", rd->file);
			rd = rd->next;
		}
	}
	else
		ft_printf("NO REDIR %i\n", ix);
}

void	ft_printcmd(t_command *cmd)
{
	int	i;

	i = 0;
	ft_printf("PRINT COMMANDS\n\n");
	while (cmd)
	{
		ft_printf("node %d\ncommand - %s\n", i, cmd->cmd);
		ft_printmatrix(cmd->args);
		ft_printrd(cmd->rd, i);
		i++;
		cmd = cmd->next;
	}
}

void	ft_printenv(t_env *lstenv)
{
	int	i;

	i = 0;
	while (lstenv)
	{
		ft_printf("node %d\n[0] - %s\n", i++, lstenv->key);
		ft_printf("[1] - %s\n", lstenv->value);
		lstenv = lstenv->next;
	}
}
