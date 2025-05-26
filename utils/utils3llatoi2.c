/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3llatoi2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aamoros- <aamoros-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:36:33 by aamoros-          #+#    #+#             */
/*   Updated: 2025/05/04 21:53:28 by aamoros-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_digits(const char **str, long long *res, int sign,
		int *error_flag)
{
	const int	digit = **str - '0';
	const bool	is_positive = (sign == 1);

	while (ft_isdigit(**str))
	{
		if (is_positive && (*res > LLONG_MAX / 10 || (*res == LLONG_MAX / 10
					&& digit > LLONG_MAX % 10)))
		{
			*res = LLONG_MAX;
			*error_flag = 1;
			return (0);
		}
		if (!is_positive && (*res > (LLONG_MAX / 10) || (*res == (LLONG_MAX
						/ 10) && digit > (LLONG_MAX % 10) + 1)))
		{
			*res = LLONG_MIN;
			*error_flag = 1;
			return (0);
		}
		*res = *res * 10 + digit;
		(*str)++;
	}
	return (1);
}

int	validate_trailing_chars(const char **str, int *error_flag)
{
	skip_whitespace(str);
	if (**str != '\0')
	{
		*error_flag = 1;
		return (0);
	}
	return (1);
}
