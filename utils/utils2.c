/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:35:02 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/30 00:35:20 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_environment(char **envp)
{
	int		count;
	char	**new_envp;
	int		i;

	count = 0;
	i = 0;
	if (!envp)
		return (NULL);
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_environment(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

void	free_environment(char **envp)
{
	free_split_array(envp);
}

void	print_syntax_error(const char *token_value)
{
	write(STDERR_FD, "minishell: syntax error near unexpected token `", 47);
	if (token_value)
		write(STDERR_FD, token_value, ft_strlen(token_value));
	else
		write(STDERR_FD, "newline", 7);
	write(STDERR_FD, "'\n", 2);
}
