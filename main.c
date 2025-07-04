/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/04 00:01:03 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	if ((*mshell)->exit_status != 0)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

static void	execute_command_pipeline(t_shell *shell)
{
	int	pid;
	int	status;

	sync_env_array(shell);
	if (execute_parent_builtin(shell->commands->args, shell))
		return ;
	pid = fork();
	if (pid == -1)
	{
		ft_error("minishell: fork");
		return ;
	}
	if (pid == 0)
		execute(shell, shell->commands->args, shell->env);
	else
    {
        // ✅ Proceso padre - capturar exit code correctamente
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status))
        {
            // ✅ Proceso terminó normalmente
            shell->last_exit_status = WEXITSTATUS(status);
        }
        else if (WIFSIGNALED(status))
        {
            // ✅ Proceso terminado por señal
            shell->last_exit_status = 128 + WTERMSIG(status);
        }
        else
        {
            // ✅ Caso por defecto
            shell->last_exit_status = 1;
        }
    }
}

static void	process_command_line(t_shell *shell)
{
	char	*input;

	input = promp_input(shell);
	if (!input)
	{
		ft_exit(&shell);
		return ;
	}
	if (input[0] != '\0')
	{
		parse_input(&shell, ft_strdup(input));
		if (shell->commands)
			execute_command_pipeline(shell);
		update_shell(&shell);
	}
	free(input);
}

static void	run_main_loop(t_shell *shell)
{
	while (shell->running)
	{
		process_command_line(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*mshell;
	int		exit_code;

	(void)argc;
	(void)argv;
	mshell = NULL;
	mshell = init_mshell(mshell, envp);
	if (!mshell)
	{
		ft_error("Fatal: Shell initialization failed.");
		return (EXIT_FAILURE);
	}
	signal_function();
	run_main_loop(mshell);
	exit_code = mshell->exit_status;
	return (exit_code);
}

/*
void	handle_pipes(t_shell *shell, char **env)
{
	t_command	*first;
	int			pid;
	int			fd[2];
	int			in_fd;
	int			cmd_count;
	int			i;
	t_command	*temp;
		int status;

	int			*pids;          // ✅ Array de PIDs específicos
	first = shell->commands;
	in_fd = STDIN_FILENO;
	// ✅ Contar comandos
	cmd_count = 0;
	temp = shell->commands;
	while (temp)
	{
		cmd_count++;
		temp = temp->next;
	}
	// ✅ Alocar array de PIDs
	pids = malloc(sizeof(int) * cmd_count);
	if (!pids)
		return (ft_error("malloc pids"));
	i = 0;
	while (shell->commands)
	{
		if (shell->commands->next && pipe(fd) < 0)
		{
			ft_error("pipe failed");
			break ;  // ✅ break para continuar limpieza
		}
		pid = fork();
		if (pid < 0)
		{
			ft_error("fork failed");
			break ;  // ✅ break para continuar limpieza
		}
		else if (pid == 0)
		{
			// ✅ Proceso hijo
			child_process(shell, fd, in_fd, env);
			// ✅ child_process hace exit internamente
		}
		else
		{
			// ✅ Proceso padre - guardar PID específico
			pids[i++] = pid;
			// ✅ Cerrar FDs correctamente
			if (shell->commands->next)
				close(fd[1]);  // Write end
			if (in_fd != STDIN_FILENO)
				close(in_fd);   // Read end anterior
			if (shell->commands->next)
				in_fd = fd[0];  // Nuevo read end
		}
		shell->commands = shell->commands->next;
	}
	// ✅ Cerrar último fd
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	// ✅ Restaurar lista
	shell->commands = first;
	// 🎯 SOLUCIÓN AL PROBLEMA: waitpid() específico
	for (int j = 0; j < i; j++)
	{
		waitpid(pids[j], &status, 0);  // ✅ Esperar PID específico
		// Opcional: manejar exit status
		if (WIFEXITED(status))
			shell->last_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_exit_status = 128 + WTERMSIG(status);
	}
	free(pids);
}

static void	child_process(t_shell *shell, int fd[2], int in_fd, char **env)
{
	int			heredoc_fd[2];
	t_command	*command;
	t_redir		*heredoc_redir;

	// ✅ Configurar señales para proceso hijo
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	command = shell->commands;
	// ✅ Manejo de exit en pipeline
	if (command && command->args && command->args[0] &&
		!ft_strcmp(command->args[0], "exit"))
	{
		ft_putendl_fd("minishell: exit: cannot be used in a pipeline",
			STDERR_FILENO);
		ft_exit_child(&shell, 0);
	}
	// ✅ Manejo de heredoc
	heredoc_redir = find_redir_by_type(command, HEREDOC);
	if (heredoc_redir)
	{
		if (pipe(heredoc_fd) < 0)
		{
			ft_error("heredoc pipe failed");
			ft_exit_child(&shell, EXIT_FAILURE);
		}
		execute_heredoc(heredoc_redir->file, heredoc_fd);
	}
	else if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	// ✅ Configurar stdout para pipe
	if (command->next)
	{
		close(fd[0]);  // No necesitamos read end
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	// ✅ Configurar redirecciones
	setup_redirection(shell, false);
	// ✅ Ejecutar comando
	exec_cmd(shell, command->args, env);
	// ✅ Nunca debería llegar aquí
	ft_exit_child(&shell, EXIT_FAILURE);
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
		input = promp_input(mshell);
		if (!input)
			ft_exit(&mshell);
		if (input[0] != '\0')
		{
			parse_input(&mshell, ft_strdup(input));
			if (mshell->commands)
			{
				sync_env_array(mshell);
				if (execute_parent_builtin(mshell->commands->args, mshell))
				{
					update_shell(&mshell);
					free(input);
					continue ;
				}
				pid = fork();
				if (pid == 0)
				{
					execute(mshell, mshell->commands->args, mshell->env);
				}
				else if (pid > 0)
				{
					waitpid(pid, &status, 0);
					// ✅ Actualizar exit status
					if (WIFEXITED(status))
						mshell->last_exit_status = WEXITSTATUS(status);
					else if (WIFSIGNALED(status))
						mshell->last_exit_status = 128 + WTERMSIG(status);
				}
				else
				{
					ft_error("fork failed");
				}
			}
			update_shell(&mshell);
		}
		free(input);
	}
	return (0);
} */

/*
	>>src/bonjour <prout >> src/hello
*/