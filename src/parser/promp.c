/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:30:35 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/07 19:00:54 by aamoros-         ###   ########.fr       */
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
		ft_free_str(&pwd);
		pwd = ft_strjoin("~", tmp);
		ft_free_str(&tmp);
	}
	ft_free_str(&home);
	return (pwd);
}

static void	validate_sigint(t_shell *mshell)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		mshell->last_exit_status = 130;
	}
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;
	char	*pwd;

	pwd = NULL;
	input = NULL;
	promp = NULL;
	pwd = ft_getenv(mshell->lstenv, "PWD");
	if (!pwd)
		pwd = ft_strdup("");
	pwd = parse_pwd(mshell, pwd);
	promp = ft_getenv(mshell->lstenv, "USER");
	promp = ft_strjoin_gnl(promp, "@mshell");
	promp = ft_strjoin_gnl(promp, pwd);
	ft_free_str(&pwd);
	promp = ft_strjoin_gnl(promp, "$ ");
	input = readline(promp);
	ft_free_str(&promp);
	validate_sigint(mshell);
	if (input && input[0] != '\0')
		add_history(input);
	return (input);
}
