/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/26 17:16:35 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	redirection_behavior(t_command *cmd)
{
	t_redir	*rd;
	int		flags;
	int		fd;

	while (cmd)
	{
		rd = cmd->rd;
		while (rd)
		{
			if (rd->type == REDIR_OUT || rd->type == APPEND)
			{
				if (rd->type == APPEND)
					flags = O_WRONLY | O_CREAT | O_APPEND;
				else
					flags = O_WRONLY | O_CREAT | O_TRUNC;
				fd = open(rd->file, flags, 0644);
				if (fd != -1)
					close(fd);
				else
					ft_printf("\x1b[31mError\x1B[37m creating: %s\n", rd->file);
			}
			else if (rd->type == REDIR_IN)
			{
				if (access(rd->file, R_OK) == -1)
				{
					ft_printf("mshell : No such file or directory : %s\n", rd->file);
				}
			}
			rd = rd->next;
		}
		cmd = cmd->next;
	}
}

void	update_shell(t_shell **mshell)
{
	// update struct() ?
	if ((*mshell)->exit_status == 1)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

int		main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	char	*input;

	(void)argc;
	(void)argv;
	mshell = NULL;
	signal_function();
	mshell = init_mshell(mshell, envp);
	if (!mshell)
		return (ft_error("init minishell"), 0);
	while (mshell->running)
	{
		mshell->user_input = input;
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0') // Si no es ENTER
		{
			parse_input(&mshell, ft_strdup(input));
			redirection_behavior(mshell->commands);
		}
		//ft_printf("main input -> %s\n", input);
		input = ft_free_str(&input);
		//ft_printenv(mshell->lstenv);
		//ft_printtkn(mshell->tkn);
		ft_printcmd(mshell->commands);
		//ft_printf("process input -> %s\n", mshell->p_input);
		update_shell(&mshell); // la funcionalidad pensada de updatear shell/env seria fuera del bucle
	}
	return (0);
}
