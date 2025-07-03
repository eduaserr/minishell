/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:22:00 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/03 19:10:56 by eduaserr         ###   ########.fr       */
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