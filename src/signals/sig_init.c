/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:35:22 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/13 20:04:52 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/* initial_signals */
volatile sig_atomic_t	g_signal_received;

static void	handle_sigint(int sign)
{
	g_signal_received = sign;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handle_sigquit(int sig)
{
	g_signal_received = sig;
	(void)sig;
}

void	signal_function(void)
{
	g_signal_received = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
