/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 14:51:13 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/01 15:21:30 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	if (big == little)
		return ((char *)little);
	while (big[i] != '\0' && i < len)
	{
		if (big[i] == little[0])
		{
			if ((i + (int)ft_strlen(little)) <= len && ft_strncmp(big + i,
					little, ft_strlen(little)) == 0)
				return ((char *)&big[i]);
			i++;
		}
		else
			i++;
	}
	return (NULL);
}
