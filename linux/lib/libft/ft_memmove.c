/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:12:13 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/15 13:05:25 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*p;

	p = (unsigned char *)src;
	ptr = (unsigned char *)dest;
	if (!p && !ptr)
	{
		return (0);
	}
	else if (dest > src)
	{
		while (n-- > 0)
		{
			ptr[n] = p[n];
		}
	}
	else
		ft_memcpy(dest, src, n);
	return ((void *)dest);
}
