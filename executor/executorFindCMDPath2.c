/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executorFindCMDPath2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:59:26 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/19 16:56:09 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_executable(char *path)
{
	struct stat	path_stat;

	if (access(path, F_OK) != 0 || access(path, X_OK) != 0)
		return (false);
	if (stat(path, &path_stat) != 0)
		return (false);
	return (!S_ISDIR(path_stat.st_mode));
}

