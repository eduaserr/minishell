/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/30 21:01:55 by eduaserr         ###   ########.fr       */
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

void	ft_exit_child(t_shell **mshell)
{
	(*mshell)->running = 0;
	(*mshell)->exit_status = 1;
	ft_free_mshell(mshell);
	ft_clean_rl(); // buena práctica
	exit(EXIT_SUCCESS);
}

static void	ft_exit2(t_shell **mshell)
{
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

void	ft_error(char *str)
{
	ft_printf("\x1b[31m%s\x1B[37m%s\n", "Error : ", str);
}

void	ft_perror(char *str, char *var)
{
	if (var[0] == '/')
		ft_printf("mshell: %s: %s\n", var, "No such file or directory");
	else
		ft_printf("%s: %s\n", var, str);
}

void	ft_error_exit(t_shell **mshell, char *message, int code)
{
	ft_error(message);
	(*mshell)->exit_status = code;
	ft_exit2(mshell);
}

/*
# define EXIT_CMD_NOT_FOUND 		127	-> no salir (nuevo promp)
command not found

# define EXIT_GENERAL_ERROR 		1	-> salir
# define EXIT_EXEC_FAILURE 			126	-> salir ?
$ chmod -x script.sh
$ ./script.sh
bash: ./script.sh: Permission denied
$ echo $?
126

# define EXIT_BUILTIN_MISUSE 		2	-> salir ?

# define SYNTAX_ERROR_STATUS 		2	-> salir ?
syntax error unexpected token ''

# define EXIT_SIGINT 				130	-> salir por señal C
# define EXIT_SIGQUIT 				131	-> no salir (se ignora)

# define EXIT_REQUEST_SIG_OFFSET 	128 + signo;	-> reflejar que el proceso
fue terminado por una señal externa.
 si un proceso muere por SIGKILL (9)
$ kill -9 PID
$ echo $?
137

# define EXIT_UQUOTE 				259	-> no salir (nuevo promp)  (exit code inventado)
*/