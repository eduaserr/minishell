/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flood_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr < eduaserr@student.42malaga.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 22:17:10 by eduaserr          #+#    #+#             */
/*   Updated: 2025/07/09 17:24:50 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*	COMENTADA PORQUE TIENE 5 ARGUMENTOS (NORMINETTE)
int	ft_flood_fill(t_data *data, char **map, int y, int x, char search)
{
	if (!map ||  y < 0 || y >= data->length
		|| x < 0 || x >= data->width || !map[y])
		return (0);
	if (map[y][x] == data->wall)
		return (0);
	if (map[y][x] == search)
		return (1);
	if (ft_flood_fill(data, map, y + 1, x, search)
		|| ft_flood_fill(data, map, y - 1, x, search)
		|| ft_flood_fill(data, map, y, x - 1, search)
		|| ft_flood_fill(data, map, y, x + 1, search))
		return (1);
	return (0);
}
 */