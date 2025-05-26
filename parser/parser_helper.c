/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:03:11 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/26 16:30:16 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees a t_redir structure
void	free_redir(void *redir_ptr)
{
	t_redir	*redir;

	redir = (t_redir *)redir_ptr;
	if (redir)
	{
		free(redir->value);
		free(redir);
	}
}

// Frees a t_command structure
void	free_command(void *command_ptr)
{
	t_command	*cmd;

	cmd = (t_command *)command_ptr;
	if (cmd)
	{
		ft_lstclear(&cmd->args_list, free);
		ft_lstclear(&cmd->redir_list, free_redir);
		free_split_array(cmd->argv);
		free(cmd);
	}
}

// Helper to create argv array from args_list
int	build_argv(t_command *cmd)
{
	int		count;
	t_list	*current;
	int		i;

	count = ft_lstsize(cmd->args_list);
	current = cmd->args_list;
	i = 0;
	cmd->argv = malloc(sizeof(char *) * (count + 1));
	if (!cmd->argv)
		return (0);
	while (current)
	{
		cmd->argv[i] = ft_strdup((char *)current->content);
		if (!cmd->argv[i])
		{
			free_split_array(cmd->argv);
			cmd->argv = NULL;
			return (0);
		}
		current = current->next;
		i++;
	}
	cmd->argv[i] = NULL;
	return (1);
}

int	is_operator_token(t_token_type type)
{
	return (type >= T_REDIR_IN && type <= T_PIPE);
}

int	is_redirection_token(t_token_type type)
{
	return (type >= T_REDIR_IN && type <= T_REDIR_HEREDOC);
}
