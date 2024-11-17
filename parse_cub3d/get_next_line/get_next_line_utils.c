/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achahid- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 08:46:16 by achahid-          #+#    #+#             */
/*   Updated: 2023/12/01 08:46:19 by achahid-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

/**
 * ft_strchr - function that locates the first occurence of c
 * @c: integer to check in ascii code
 * @s: pointer to the string to search in
 * Return: 1 if c is located in s, otherwise 0
*/
int	ft_strchr(const char *s, int c)
{
	size_t			count;
	unsigned char	w;

	w = (unsigned char)c;
	if (s == NULL)
		return (0);
	if (*s == '\0' && w == '\0')
		return (1);
	count = 0;
	while (*(s + count))
	{	
		if (*(s + count) == w)
			return (1);
		count++;
		if (!(*(s + count)) && !w)
			return (1);
	}
	return (0);
}

static void	ft_strcpy(char *dst, char *src);

/**
 * ft_strjoin - function that concatenates s1 and s2
 * and allocate memory for the new string.
 * @s1: The prefix string
 * @s2: The suffix string
 * Return: The new string, othewise NULL
*/
char	*ft_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	len_s1;
	size_t	len_s2;
	int		len;

	len_s1 = 0;
	len_s2 = 0;
	if (s1 != NULL)
		len_s1 = ft_strlen(s1);
	if (s2 != NULL)
		len_s2 = ft_strlen(s2);
	len = len_s1 + len_s2;
	new_str = malloc(sizeof(*new_str) * len + 1);
	if (new_str == NULL)
		return (NULL);
	if (s1 != NULL)
		ft_strcpy(new_str, s1);
	if (s2 != NULL)
		ft_strcpy(new_str + len_s1, s2);
	free(s1);
	return (new_str);
}

/**
 * ft_strcpy - function that copies bytes's src into dst
 * @src: source
 * @dst: destination
 * Return: void.
*/
static void	ft_strcpy(char *dst, char *src)
{
	int	count;

	count = 0;
	while (src[count])
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = '\0';
}

/**
 * ft_strlen - function that counts the lenght of str
 * @str: pointer to string
 * Return: length of str
*/
size_t	ft_strlen(char *str)
{
	size_t	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}
