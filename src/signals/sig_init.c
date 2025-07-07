/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:35:22 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/07 19:29:21 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_signal_received;

static void	handle_sigint(int sign)
{
	(void)sign;
	g_signal_received = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_function(void)
{
	g_signal_received = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
}

static void	handle_sigquit(int sign)
{
	(void)sign;
	ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
}

void	handle_exec_sigquit(void)
{
	g_signal_received = 0;
	signal(SIGQUIT, handle_sigquit);
	signal(SIGINT, SIG_IGN);
}
