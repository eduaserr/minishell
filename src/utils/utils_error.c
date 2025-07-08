/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/08 21:23:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_clean_rl(void)
{
	rl_clear_history();
	rl_free_line_state();
	rl_deprep_terminal();
	rl_cleanup_after_signal();
}

void	ft_exit(t_shell **mshell)
{
	(*mshell)->running = 0;
	(*mshell)->exit_status = 1;
	ft_free_mshell(mshell);
	ft_clean_rl();
	write(STDERR_FILENO, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

void	ft_exit_child(t_shell **mshell, int code)
{
	(*mshell)->running = 0;
	(*mshell)->exit_status = 1;
	ft_free_mshell(mshell);
	ft_clean_rl();
	exit(code);
}
