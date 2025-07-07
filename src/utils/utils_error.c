/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/07 18:38:41 by aamoros-         ###   ########.fr       */
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

static void	ft_exit2(t_shell **mshell)
{
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

void	ft_error_exit(t_shell **mshell, char *message, int code)
{
	ft_error(message);
	(*mshell)->exit_status = code;
	ft_exit2(mshell);
}
