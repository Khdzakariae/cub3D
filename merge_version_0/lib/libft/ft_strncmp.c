/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 09:46:50 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/12 11:36:54 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((i < n) && (s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
		{
			break ;
		}
		i++;
	}
	if (i == n || (s1[i] == '\0' && s2[i] == '\0'))
	{
		return (0);
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
