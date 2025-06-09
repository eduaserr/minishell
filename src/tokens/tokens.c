/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 15:39:15 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/06 17:43:18 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// EN CONSTRUCCION //

/* tokenizer()
if (str[i] == '<')
	add_token(str, i, REDIR_IN)
else if (str[i] == '<' && str[i + 1] == '<')
	add_token(str, i, HEREDOC) */


/* 	ls -la
	command [0] - ls -la
	args[0][0] == "ls"
	token
		type = WORD
		value "ls"
	REDIR
	type = NULL
	value = NULL
	


		ls -l -a > txt.txt
	command [0] - ls -la > txt.txt
	args[0][0] == "ls"
	args[0][1] == "-la"
	token
		type = WORD
		value "ls"
	REDIR
	type = REDIR_OUT
	value = ">" */