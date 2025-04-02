/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/04/02 22:21:40 by eduaserr         ###   ########.fr       */
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
	ft_printf("exit\n");
	//ft_clean_rl();
	ft_free_mshell(mshell);
	exit(EXIT_SUCCESS);
}

void	ft_error(char *str)
{
	ft_printf("\x1b[31m%s\x1B[37m%s\n", "Error : ", str);
}
