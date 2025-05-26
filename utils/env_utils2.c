/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 21:31:34 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/09 21:31:53 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env_var(char *name, char *value, char ***envp_ptr)
{
	int		index;
	char	*new_entry_str;

	new_entry_str = create_env_entry_string(name, value);
	if (!new_entry_str)
		return (1);
	index = find_env_var_index(name, *envp_ptr);
	if (index >= 0)
		update_existing_variable(*envp_ptr, index, new_entry_str);
	else
	{
		if (add_new_variable_to_env(envp_ptr, new_entry_str) != 0)
			return (1);
	}
	return (0);
}

int	remove_env_var(const char *name, char ***envp_ptr)
{
	int		index;
	int		i;
	char	**envp;

	envp = *envp_ptr;
	if (!envp)
		return (0);
	index = find_env_var_index(name, envp);
	if (index == -1)
		return (0);
	free(envp[index]);
	i = index;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	return (0);
}

int	unset_env_var(const char *name, char ***envp_ptr)
{
	int		index;
	int		i;
	char	**envp;
	char	**smaller_envp;
	int		count;

	envp = *envp_ptr;
	if (!envp)
		return (0);
	index = find_env_var_index(name, envp);
	if (index == -1)
		return (0);
	free(envp[index]);
	i = index;
	while (envp[i])
	{
		envp[i] = envp[i + 1];
		i++;
	}
	count = i - 1;
	smaller_envp = ft_realloc(envp, sizeof(char *) * (count + 1));
	if (!smaller_envp && count > 0)
		return (1);
	*envp_ptr = smaller_envp;
	return (0);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}
