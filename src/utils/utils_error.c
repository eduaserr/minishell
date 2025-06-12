/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/12 21:51:40 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//por implementar, pone por default la salida de las señales
/* void	reset_signal_handlers(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
} */

void	ft_clean_rl(void)
{
	// Limpia el historial de readline
	rl_clear_history();

	// Libera el estado de la línea actual
	rl_free_line_state();

	// Limpia la configuración de la terminal
	rl_deprep_terminal();

	// Limpia recursos internos de readline
	rl_cleanup_after_signal();
}

void	ft_exit(t_shell **mshell)
{
	(*mshell)->running = 0;
	(*mshell)->exit_status = 1;
	ft_free_mshell(mshell);
	ft_clean_rl(); // buena práctica
	write(STDERR_FILENO, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

static void	ft_exit2(t_shell **mshell)
{
	if ((*mshell)->exit_status == 1)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

void	ft_error(char *str)
{
	ft_printf("\x1b[31m%s\x1B[37m%s\n", "Error : ", str);
}

void ft_error_exit(t_shell **mshell, char *message, int code)
{
	ft_error(message);
	(*mshell)->exit_status = code;
	ft_exit2(mshell);
}
