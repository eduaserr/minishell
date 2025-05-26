/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3llatoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:36:09 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/12 20:11:00 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long	ft_llatoi_strict(const char *str, int *error_flag)
{
	long long	result;
	int			sign;

	*error_flag = 0;
	result = 0;
	sign = 1;
	if (handle_null_input(str, error_flag))
		return (0);
	skip_whitespace(&str);
	parse_sign(&str, &sign);
	if (!validate_digit_presence(&str, error_flag))
		return (0);
	if (!process_digits(&str, &result, sign, error_flag))
	{
		if (sign == 1)
			return (LLONG_MAX);
		else
			return (LLONG_MIN);
	}
	if (!validate_trailing_chars(&str, error_flag))
		return (0);
	return (result * sign);
}

int	handle_null_input(const char *str, int *error_flag)
{
	if (!str)
	{
		*error_flag = 1;
		return (1);
	}
	return (0);
}

void	skip_whitespace(const char **str)
{
	while (**str == ' ' || (**str >= '\t' && **str <= '\r'))
		(*str)++;
}

void	parse_sign(const char **str, int *sign)
{
	if (**str == '+' || **str == '-')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

int	validate_digit_presence(const char **str, int *error_flag)
{
	if (!ft_isdigit(**str))
	{
		*error_flag = 1;
		return (0);
	}
	return (1);
}
