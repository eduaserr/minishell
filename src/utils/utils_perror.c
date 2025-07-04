/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_perror.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:59:26 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/04 14:20:21 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_error(char *str)
{
	ft_printf("\x1b[31m%s\x1B[37m%s\n", "Error : ", str);
}

void	ft_perror(t_shell *shell, char *str, char *var)
{
	if (ft_strcmp(str, "nsfod") == 0)
	{
		shell->last_exit_status = 127;
		ft_printf("mshell: %s: %s\n", var, "No such file or directory");
	}
	else if (ft_strcmp(str, "senut") == 0)
	{
		shell->last_exit_status = 2;
		ft_printf("syntax error near unexpected token `%s'\n", var);
	}
	else if (ft_strcmp(str, "cnf") == 0)
	{
		shell->last_exit_status = 127;
		ft_printf("%s: %s\n", var, "command not found");
	}
	else
		ft_printf("%s: %s\n", var, str);
}
