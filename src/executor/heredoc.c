/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 20:04:44 by aamoros-          #+#    #+#             */
/*   Updated: 2025/07/03 20:09:47 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*void	execute_heredoc(char *delimiter, int heredoc_fd[2])
{
	char	*line;

	setup_heredoc_signals();
	while (g_signal_received != SIGINT)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(heredoc_fd[1], line, ft_strlen(line));
		write(heredoc_fd[1], "\n", 1);
		free(line);
	}
	close(heredoc_fd[1]);
}*/

/* static void	handle_file_input(t_shell *shell, char *file)
{
	int	fd_in;

	(void)shell;
	fd_in = open(file, O_RDONLY);
	if (fd_in == -1)
		return (ft_perror("No such file or directory", file),
			ft_exit_child(&shell, 1));
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
} */
