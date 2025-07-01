/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 19:19:24 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/01 20:27:51 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//aún por añadir y comprobar funcionamiento
/* heredoc signals */
static void	handle_heredoc_sigint(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
}

/* exec signals */
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

//por implementar, pone por default la salida de las señales
void	signal_default(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
}
