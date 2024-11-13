/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 17:51:52 by zel-khad          #+#    #+#             */
/*   Updated: 2024/02/11 17:51:54 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_line(char *line)
{
	char	*ret;
	int		i;
	int		j;

	if (!*line)
		return (NULL);
	if (!ft_strchr(line, 10))
	{
		ret = ft_substr(line, 0, ft_strlen(line));
		free(line);
		line = NULL;
		return (ret);
	}
	i = ft_strlen(line);
	j = ft_strlen(ft_strchr(line, 10));
	ret = ft_substr(line, 0, i - j + 1);
	free(line);
	return (ret);
}

char	*ft_return(char *str)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	if (!*str)
		return (NULL);
	if (!ft_strchr(str, 10))
		return (0);
	tmp = str;
	i = ft_strlen(str);
	j = ft_strlen(ft_strchr(str, 10));
	str = ft_substr(tmp, i - j + 1, j);
	return (str);
}

char	*ft_read(char *str, int fd)
{
	ssize_t	i;
	char	*buf;

	i = 1;
	buf = malloc((BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	while (!ft_strchr(str, 10) && i != 0)
	{
		i = read(fd, buf, BUFFER_SIZE);
		if (i == -1)
		{
			free(buf);
			free(str);
			return (NULL);
		}
		buf[i] = '\0';
		str = ft_strjoin(str, buf);
	}
	free(buf);
	if (i == 0 && (str[0] == '\0'))
		return (free(str), NULL);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*str ;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	line = ft_read(str, fd);
	if (!line)
	{
		str = NULL;
		return (NULL);
	}
	str = ft_return(line);
	line = ft_line(line);
	return (line);
}
