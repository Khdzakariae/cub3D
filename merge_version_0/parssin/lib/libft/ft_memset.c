/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:25:24 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/12 11:45:34 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;
	unsigned char	p;

	i = 0;
	p = (unsigned char)c;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		ptr[i] = p;
		i++;
	}
	return ((void *)s);
}
