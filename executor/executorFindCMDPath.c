/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executorFindCMDPath.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:53:33 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/19 19:43:18 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *command)
{
	char	**paths;
	char	*result;

	if (!command || !*command)
		return (NULL);
	if (ft_strchr(command, '/'))
		return (handle_absolute_path(command));
	paths = get_path_directories();
	if (!paths)
		return (NULL);
	result = search_in_path_directories(paths, command);
	free_split_array(paths);
	return (result);
}

char	*handle_absolute_path(char *command)
{
	struct stat	path_stat;

	if (stat(command, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		return (NULL);
	if (is_valid_executable(command))
		return (ft_strdup(command));
	return (NULL);
}

char	**get_path_directories(void)
{
	char	*path_env;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

char	*search_in_path_directories(char **paths, char *command)
{
	int		i;
	char	*full_path;

	i = -1;
	while (paths[++i])
	{
		full_path = check_directory_path(paths[i], command);
		if (full_path)
			return (full_path);
	}
	return (NULL);
}

char	*check_directory_path(char *dir, char *command)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strj(dir, "/");
	if (!temp_path)
		return (NULL);
	full_path = ft_strj(temp_path, command);
	free(temp_path);
	if (!full_path)
		return (NULL);
	if (is_valid_executable(full_path))
		return (full_path);
	free(full_path);
	return (NULL);
}
