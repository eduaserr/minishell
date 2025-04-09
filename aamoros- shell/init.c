/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 15:25:56 by aamoros-          #+#    #+#             */
/*   Updated: 2025/04/04 15:25:57 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env_list = create_env_list(envp);
	if (!shell->env_list)
	{
		free(shell);
		return (NULL);
	}
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	shell->running = 1;
	return (shell);
}

void	free_env_list(t_env *list)
{
	t_env	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->value);
		free(temp);
	}
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*temp;

	while (redirs)
	{
		temp = redirs;
		redirs = redirs->next;
		free(temp->file);
		free(temp);
	}
}

void	free_commands(t_command *commands)
{
	t_command	*temp;
	int			i;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		if (temp->args)
		{
			i = 0;
			while (temp->args[i])
			{
				free(temp->args[i]);
				i++;
			}
			free(temp->args);
		}
		if (temp->redirs)
			free_redirs(temp->redirs);
		free(temp);
	}
}

void	free_shell(t_shell *shell)
{
	if (shell)
	{
		if (shell->env_list)
			free_env_list(shell->env_list);
		if (shell->tokens)
			free_tokens(shell->tokens);
		if (shell->commands)
			free_commands(shell->commands);
		free(shell);
	}
}
