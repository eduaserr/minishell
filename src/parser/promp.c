/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   promp.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 21:30:35 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/27 20:18:09 by aamoros-         ###   ########.fr       */
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
	home = ft_free_str(&home);
	return (pwd);
}

char	*promp_input(t_shell *mshell)
{
	char	*promp;
	char	*input;
	char	*pwd;
	char	cwd[4096];

	pwd = NULL;
	input = NULL;
	promp = NULL;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		pwd = ft_strdup(cwd);
	else
		pwd = ft_strdup("?");
	pwd = parse_pwd(mshell, pwd);
	promp = ft_getenv(mshell->lstenv, "USER");
	promp = ft_strjoin_gnl(promp, "@mshell");
	promp = ft_strjoin_gnl(promp, pwd);
	pwd = ft_free_str(&pwd);
	promp = ft_strjoin_gnl(promp, "$ ");
	input = readline(promp);
	promp = ft_free_str(&promp);
	if (!input)
		return (NULL);
	if (input && input[0] != '\0')
		add_history(input);
	return (input);
}
