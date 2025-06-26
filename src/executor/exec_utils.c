/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 17:09:25 by aamoros-          #+#    #+#             */
/*   Updated: 2025/06/26 16:16:25 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*find_cmd_path(char **paths, char *cmd)
{
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	return (path);
}

char	*get_cmd_paths(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
		i++;
	}
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	path = find_cmd_path(paths, cmd);
	ft_free_array((void **)paths);
	return (path);
}
