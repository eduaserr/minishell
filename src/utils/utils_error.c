/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/29 19:31:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	(*mshell)->exit_status = 1;
	(*mshell)->running = 0;
	write(STDERR_FILENO, "exit\n", 5);
	ft_clean_rl(); // buena práctica
	ft_free_mshell(mshell);
	exit(EXIT_SUCCESS);
}

void	ft_error(char *str)
{
	ft_printf("\x1b[31m%s\x1B[37m%s\n", "Error : ", str);
}

/* NEW FT_ERROR FUNCTION
 Add this to minishell.h
#define ERR_SYNTAX 2
#define ERR_CMD_NOT_FOUND 127
#define ERR_PERMISSION 126
#define ERR_GENERAL 1

// Enhance your error function
void ft_error_exit(t_shell **mshell, char *message, int code)
{
	ft_printf("\x1b[31mError: %s\x1B[37m\n", message);
	(*mshell)->exit_status = code;

	// Only exit shell for critical errors
	if (code == EXIT_FAILURE)
		ft_exit(mshell);
} */