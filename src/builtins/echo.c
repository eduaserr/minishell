/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:54:35 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/17 18:02:44 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	parse_echo_flags(char **args, int *suppress_newline)
{
	int		i;
	char	*p;

	i = 1;
	*suppress_newline = 0;
	while (args && args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		p = args[i] + 1;
		while (*p == 'n')
			p++;
		if (*p == '\0')
		{
			*suppress_newline = 1;
			i++;
		}
		else
			break ;
	}
	return (i);
}

int	builtin_echo(t_command *cmd)
{
	int		i;
	int		suppress_newline;
	char	**args;

	args = cmd->args;
	i = parse_echo_flags(args, &suppress_newline);
	while (args && args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FD);
		i++;
		if (args[i])
			write(STDOUT_FD, " ", 1);
	}
	if (!suppress_newline)
		write(STDOUT_FD, "\n", 1);
	return (0);
}
