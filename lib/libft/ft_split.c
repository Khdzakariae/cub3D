/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:16:42 by zel-khad          #+#    #+#             */
/*   Updated: 2023/11/15 13:29:14 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_clear(char **ptr, int current)
{
	int	i;

	i = 0;
	while (i < current)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

static int	checkwords(char const *s, char c)
{
	size_t	i;
	int		words;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			while (s[i] && s[i] != c)
			{
				i++;
			}
			words++;
			continue ;
		}
		i++;
	}
	return (words);
}

static int	fill_arrays(char const *s, char c, char **ptr, int end)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			i++;
			continue ;
		}
		else
		{
			end = i;
			while (s[end] && s[end] != c)
				end++;
			ptr[j] = ft_substr(s, i, end - i);
			if (ptr[j] == NULL)
				return (ft_clear(ptr, j), 1);
			j++;
			i = end;
		}
	}
	return (ptr[j] = NULL, 0);
}

char	**ft_split(char const *s, char c)
{
	int		words;
	char	**ptr;

	if (!s)
		return (NULL);
	words = checkwords(s, c) + 1;
	ptr = malloc(words * sizeof(char *));
	if (!ptr)
		return (NULL);
	if (fill_arrays(s, c, ptr, 0))
		return (NULL);
	return (ptr);
}
