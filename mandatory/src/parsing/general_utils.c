/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:09:51 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 13:28:46 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

t_bool	space_checker(char *line);
t_bool	ft_isspace(char c);
int		get_next_line_len(int fd, char **line, int *len);

t_bool	space_checker(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			return (true);
		if (ft_isspace(*line) == false)
			return (false);
		line++;
	}
	return (true);
}

t_bool	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

int	get_next_line_len(int fd, char **line, int *len)
{
	*len = get_next_line(fd, line);
	return (*len);
}

/* void	print_tmc(t_cub3d *cub3d)
{
	// check paths
	printf("NO: %s\n", cub3d->textures.no.path);
	printf("SO: %s\n", cub3d->textures.so.path);
	printf("WE: %s\n", cub3d->textures.we.path);
	printf("EA: %s\n", cub3d->textures.ea.path);
	// check colors
	printf("F: %x\n", cub3d->colors.floor.color);
	printf("C: %x\n", cub3d->colors.ceiling.color);
	printf("Map width: %d\n", cub3d->map.width);
    printf("Map height: %d\n", cub3d->map.height);
    // print map
    for (size_t i = 0; (int)i < cub3d->map.height; i++)
        printf("%s\n", cub3d->map.map[i]); 
} */
