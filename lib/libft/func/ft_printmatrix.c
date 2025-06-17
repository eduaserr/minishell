/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printmatrix.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:30:30 by eduaserr          #+#    #+#             */
/*   Updated: 2025/06/17 01:58:44 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_printmatrix(char **file_matrix)
{
	int	i;

	i = -1;
	while (file_matrix && file_matrix[++i])
		ft_printf("[%d] : %s\n", i, file_matrix[i]);
}
