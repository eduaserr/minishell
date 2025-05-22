/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rm_db_spaces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduaserr <eduaserr@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 04:24:56 by eduaserr          #+#    #+#             */
/*   Updated: 2025/05/22 05:28:36 by eduaserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

// rm_double_spaces. Remove all double spaces and returns new malloc str

static int	ft_strlendb(char *str, int space, int i)
{
	int		len;

	len = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
		{
			if (!space)
			{
				len++;
				space = 1;
			}
		}
		else
		{
			len++;
			space = 0;
		}
		i++;
	}
	if (len > 0 && ft_isspace(str[i - 1]))
		len--;
	return (len);
}

static char	*ft_strcpydb(char *str, char *tmp, int space, int i, int len)
{
	int	j;

	j = 0;
	while (str[i] && j < len)
    {
        if (ft_isspace(str[i]))
        {
            if (!space)
            {
                tmp[j++] = ' ';
                space = 1;
            }
        }
        else
        {
            tmp[j++] = str[i];
            space = 0;
        }
        i++;
    }
    tmp[j] = '\0';
	return (tmp);
}

char	*ft_rm_db_spaces(char **str)
{
	char	*tmp;
	int		i;
	int		len;
	int		space;

	if (!str || !*str)
		return (NULL);
	i = 0;
	len = 0;
	space = 1;
	len = ft_strlendb(*str, space, i);
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (!tmp)
		return (NULL);
	tmp = ft_strcpydb(*str, tmp, space, i, len);
	ft_free_str(str);
	return (tmp);
}

/*char	*ft_rm_db_spaces(char **str)
{
    char	*tmp;
    int		i;
    int		j;
    int		wrote_char;

    if (!str || !*str)
        return (NULL);
	i = 0;
    j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			j++;
		i++;
	}
	i = i - j;
    tmp = (char *)malloc(sizeof(char) * (i + 1));
    if (!tmp)
        return (NULL);

    wrote_char = 0;
    // Saltar espacios iniciales
    while ((*str)[i] && ft_isspace((*str)[i]))
        i++;
    while ((*str)[i])
    {
        if (!ft_isspace((*str)[i]))
        {
            tmp[j++] = (*str)[i];
            wrote_char = 1;
        }
        else if (wrote_char)
        {
            // Saltar todos los espacios consecutivos
            while ((*str)[i] && ft_isspace((*str)[i]))
                i++;
            // Si no es el final, añade un solo espacio
            if ((*str)[i])
                tmp[j++] = ' ';
            i--; // Para compensar el incremento extra del bucle principal
        }
        i++;
    }
    tmp[j] = '\0';
    ft_free_str(str);
    return (tmp);
}*/