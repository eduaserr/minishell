/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:30:35 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/30 19:19:58 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	ft_free_str(&home);
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
/* 	if (g_signal_received == SIGINT)
		mshell->last_exit_status = EXIT_SIGINT; */
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
