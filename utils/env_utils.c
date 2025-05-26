/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 22:29:46 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/09 21:32:26 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_var_name(const char *name)
{
	if (!name || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	while (*name)
	{
		if (!ft_isalnum(*name) && *name != '_')
			return (0);
		name++;
	}
	return (1);
}

int	find_env_var_index(const char *name, char **envp)
{
	int		i;
	size_t	name_len;

	i = 0;
	if (!envp || !name)
		return (-1);
	name_len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*create_env_entry_string(char *name, char *value)
{
	char	*temp_name_eq;
	char	*new_entry;

	temp_name_eq = ft_strjoin(name, "=");
	if (!temp_name_eq)
		return (NULL);
	if (value)
		new_entry = ft_strjoin(temp_name_eq, value);
	else
		new_entry = ft_strjoin(temp_name_eq, "");
	free(temp_name_eq);
	return (new_entry);
}

void	update_existing_variable(char **env_array, int index,
		char *new_entry_string)
{
	free(env_array[index]);
	env_array[index] = new_entry_string;
}

int	add_new_variable_to_env(char ***envp_ptr, char *new_entry_string)
{
	size_t	count;
	char	**new_envp_array;
	char	**current_envp;
	size_t	i;

	current_envp = *envp_ptr;
	count = 0;
	while (current_envp && current_envp[count])
		count++;
	new_envp_array = malloc(sizeof(char *) * (count + 2));
	if (!new_envp_array)
		return (free(new_entry_string), 1);
	i = 0;
	while (i < count)
	{
		new_envp_array[i] = current_envp[i];
		i++;
	}
	new_envp_array[count] = new_entry_string;
	new_envp_array[count + 1] = NULL;
	free(current_envp);
	*envp_ptr = new_envp_array;
	return (0);
}
