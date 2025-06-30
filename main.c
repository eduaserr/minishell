/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/30 20:04:52 by aamoros-         ###   ########.fr       */
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
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	char	*input;
	int		pid;
	int		status;

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
		input = promp_input(mshell);
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0')
		{
			parse_input(&mshell, ft_strdup(input));
			ft_printcmd(mshell->commands);
			if (mshell->commands)
			{
				if (execute_parent_builtin(mshell->commands->args, mshell))
				{
					update_shell(&mshell);
					free(input);
					continue ;
				}
				pid = fork();
				if (pid == 0)
					execute(mshell, mshell->commands->args, mshell->env);
				waitpid(pid, &status, 0);
			}
			update_shell(&mshell);
		}
		free(input);
	}
	return (0);
}
