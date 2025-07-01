/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 13:12:54 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/01 17:23:22 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_shlvl(t_env *lstenv)
{
	int		shlvl;
	char	*new_shlvl_str;

	lstenv = ft_getlstenv(lstenv, "SHLVL");
	if (!lstenv)
		return (ft_error("update shlvl"));
	shlvl = ft_atoi(lstenv->value);
	shlvl++;
	new_shlvl_str = ft_itoa(shlvl);
	if (new_shlvl_str)
	{
		free(lstenv->value);
		lstenv->value = new_shlvl_str;
	}
}

static char	**new_env(int i)
{
	char	**env;
	char	*pwd;

	pwd = NULL;
	env = NULL;
	env = (char **)ft_calloc(i, sizeof(char *));
	if (!env)
		return (NULL);
	env[0] = ft_strdup("USER=");
	if (!env[0])
		return (free(env), NULL);
	env[1] = ft_strdup("HOME=");
	if (!env[1])
		return (ft_freematrix(&env), NULL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (ft_freematrix(&env), NULL);
	env[2] = ft_strjoin("PWD=", pwd);
	ft_free_str(&pwd);
	if (!env[2])
		return (ft_freematrix(&env), NULL);
	env[3] = ft_strdup("SHLVL=0");
	if (!env[3])
		return (ft_freematrix(&env), NULL);
	return (env);
}

static t_env	*init_env(t_env *lstenv, char **env)
{
	t_env	*new;
	int		i;

	lstenv = NULL;
	new = NULL;
	i = 0;
	while (env && env[i])
	{
		new = create_env(env[i]);
		if (!new)
			return (NULL);
		addlast_node(&lstenv, new);
		i++;
	}
	return (lstenv);
}

/* static t_env	*find_env_node(t_env *lstenv, char *key)
{
	t_env	*current;
	char	**new_env;
	int		count;
	int		i;
	t_env	*shlvl_node;
	char	*current_shlvl;
	int		shlvl_value;
	char	*new_shlvl;
	int		i;
	t_env	*new_node;

	current = lstenv;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
} */
/* static void	add_to_env_array(t_shell *mshell, char *env_var)
{
	if (!mshell || !env_var)
		return ;
	count = 0;
	while (mshell->env && mshell->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return ;
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(mshell->env[i]);
		if (!new_env[i])
		{
			while (--i >= 0)
				free(new_env[i]);
			free(new_env);
			return ;
		}
		i++;
	}
	new_env[count] = ft_strdup(env_var);
	if (!new_env[count])
	{
		while (--i >= 0)
			free(new_env[i]);
		free(new_env);
		return ;
	}
	new_env[count + 1] = NULL;
	ft_freematrix(&mshell->env);
	mshell->env = new_env;
} */
/* static void	update_SHLVL(t_shell *mshell)
{
	shlvl_node = find_env_node(mshell->lstenv, "SHLVL");
	if (shlvl_node)
	{
		current_shlvl = shlvl_node->value;
		shlvl_value = ft_atoi(current_shlvl);
		shlvl_value++;
		new_shlvl = ft_itoa(shlvl_value);
		if (!new_shlvl)
			return ;
		free(shlvl_node->value);
		shlvl_node->value = new_shlvl;
		i = 0;
		while (mshell->env && mshell->env[i])
		{
			if (ft_strncmp(mshell->env[i], "SHLVL=", 6) == 0)
			{
				free(mshell->env[i]);
				mshell->env[i] = ft_strjoin("SHLVL=", new_shlvl);
				break ;
			}
			i++;
		}
	}
	else
	{
		new_node = create_env("SHLVL=1");
		if (new_node)
			addlast_node(&mshell->lstenv, new_node);
		add_to_env_array(mshell, "SHLVL=1");
	}
} */
t_shell	*init_mshell(t_shell *mshell, char **envp)
{
	mshell = (t_shell *)malloc(sizeof(t_shell));
	if (!mshell)
		return (NULL);
	mshell->cmd_count = 0;
	mshell->running = 1;
	mshell->exit_status = 0;
	mshell->last_exit_status = 0;
	mshell->commands = NULL;
	mshell->tkn = NULL;
	mshell->p_input = NULL;
	mshell->user_input = NULL;
	if (!envp || !*envp)
		mshell->env = new_env(5);
	else
		mshell->env = ft_init_array(envp);
	if (!mshell->env)
		return (free(mshell), NULL);
	mshell->lstenv = init_env(mshell->lstenv, mshell->env);
	if (!mshell->lstenv)
		return (ft_free_mshell(&mshell), NULL);
	update_shlvl(mshell->lstenv);
	return (mshell);
}

void	sync_lstenv_from_env(t_shell *shell)
{
	int		i;
	t_env	*new_list;
	t_env	*new_node;
	t_env	*tmp;

	new_list = NULL;
	while (shell->lstenv)
	{
		tmp = shell->lstenv->next;
		free(shell->lstenv->key);
		free(shell->lstenv->value);
		free(shell->lstenv);
		shell->lstenv = tmp;
	}
	i = 0;
	while (shell->env && shell->env[i])
	{
		new_node = create_env(shell->env[i]);
		if (new_node)
			addlast_node(&new_list, new_node);
		i++;
	}
	shell->lstenv = new_list;
}
