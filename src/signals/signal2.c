/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:19:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/08 21:21:40 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_heredoc_sigint(int sig)
{
	g_signal_received = sig;
	rl_done = 1;
}

void	setup_heredoc_signals(void)
{
	g_signal_received = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_heredoc_sigint);
}

static void	sigquit_handler(int signal)
{
	(void)signal;
	ft_putendl_fd("Quit (core dumped)", STDOUT_FILENO);
}

void	init_signal_handler_exec(void)
{
	g_signal_received = 0;
	signal(SIGQUIT, sigquit_handler);
	signal(SIGINT, SIG_IGN);
}

void	sigint_heredoc_handler(int sig)
{
	if (g_signal_received != -1)
		close(g_signal_received);
	exit(128 + sig);
}
