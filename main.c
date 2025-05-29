/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:27 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/29 22:05:05 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	update_shell(t_shell **mshell)
{
	//update_struct();
	if ((*mshell)->exit_status)
		(*mshell)->running = 0;
	ft_free_mshell(mshell);
}

static char	*parse_pwd(t_shell *mshell, char *pwd)
{
	char	*home;
	char	*tmp;

	tmp = NULL;
	home = NULL;
	home = ft_getenv(mshell->lstenv, "HOME");
	if (!home)
		return (pwd);
	if (ft_strncmp(pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_substr(pwd, ft_strlen(home), ft_strlen(pwd) - ft_strlen(home));
		pwd = ft_free_str(&pwd);
		pwd = ft_strjoin("~", tmp);
		tmp = ft_free_str(&tmp);
	}
	home = ft_free_str(&home);
	return (pwd);
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;
	char	*pwd;

	pwd = NULL;
	input = NULL;
	promp = NULL;
	pwd = ft_getenv(mshell->lstenv, "PWD");		// /home/eduaserr/cursus/minishell
	pwd = parse_pwd(mshell, pwd);
	promp = ft_getenv(mshell->lstenv, "USER");	//eduaserr
	promp = ft_strjoin_gnl(promp, "@mshell");	//eduaserr@mshell
	promp = ft_strjoin_gnl(promp, pwd);			//eduaserr@mshell/home/eduaserr/cursus/minishell
	pwd = ft_free_str(&pwd);
	promp = ft_strjoin_gnl(promp, "$ ");		//eduaserr@mshell/home/eduaserr/cursus/minishell$ 
	input = readline(promp);
	promp = ft_free_str(&promp);
	if (!input)
		return (NULL);
	if (input && input[0] != '\0')
		add_history(input);
	return (input);
}

int	main(int argc, char **argv, char **envp)
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
		input = promp_input(mshell); //Ctrl + D signal se maneja con readline EOF
		if (!input)
			ft_error_exit(&mshell, NULL, 1);
		if (input[0] != '\0') // Si no es ENTER
			parse_input(&mshell, ft_strdup(input));
		ft_printf("main input -> %s\n", input);
		input = ft_free_str(&input);
		//ft_printenv(mshell->lstenv);
		ft_printcmd(mshell->commands);
		ft_printf("process input -> %s\n", mshell->p_input);
		update_shell(&mshell);
		//updateshlvl();
	}
	return (0);
}



/*
	exit_status : esto nos permite poder hacer malloc fuera del while(1)
	y liberar solo cuando sea necesario :

- exit_status :
	ft_exit(int , mshell) con condiciones para liberar distintos elementos de mshell.

- valgrind errors = al ejecutar minishell, printear un input (a) y sea el siguiente input ENTER
(probablemente se arregle al avanzar el proyecto)




eserraga@mshell$ a ver con esto | |
Error : split error
Error : split input
Error : Parse command
Error : get command
main input -> a ver con esto | |
	*/