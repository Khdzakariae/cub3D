/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:56:12 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/15 13:19:59 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	*p;

	p = (unsigned char *)src;
	ptr = (unsigned char *)dest;
	i = 0;
	if (!p && !ptr)
		return (NULL);
	if ((dest == src) && n != 0)
		return (dest);
	while (i < n)
	{
		ptr[i] = p[i];
		i++;
	}
	return (dest);
}
