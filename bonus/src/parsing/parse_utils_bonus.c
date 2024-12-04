/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:45 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 13:29:06 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_bonus.h>

t_bool	check_cub_extension(char *file);
void	init_cub3d_data(t_cub3d **cub3d);
void	fetch_min_map_width(char **map, size_t *min_width);
void	get_color(char *line, int *color, t_cub3d **cub3d_data);
void	retrieve_color(char **split_colors, int *color, t_cub3d **cub3d_data);

t_bool	check_cub_extension(char *file)
{
	while (file && *file)
	{
		if (*file == '.' && *(file + 1) == 'c' && *(file + 2) == 'u'
			&& *(file + 3) == 'b' && *(file + 4) == '\0')
			return (true);
		file++;
	}
	return (false);
}

void	init_cub3d_data(t_cub3d **cub3d)
{
	(*cub3d)->textures.no.path = NULL;
	(*cub3d)->textures.no.is_set = false;
	(*cub3d)->textures.so.path = NULL;
	(*cub3d)->textures.so.is_set = false;
	(*cub3d)->textures.we.path = NULL;
	(*cub3d)->textures.we.is_set = false;
	(*cub3d)->textures.ea.path = NULL;
	(*cub3d)->textures.ea.is_set = false;
	(*cub3d)->colors.floor.color = -1;
	(*cub3d)->colors.floor.is_set = false;
	(*cub3d)->colors.ceiling.color = -1;
	(*cub3d)->colors.ceiling.is_set = false;
	(*cub3d)->map.map = NULL;
	(*cub3d)->map.width = 0;
	(*cub3d)->map.height = 0;
	(*cub3d)->map.tile_size = TILE_SIZE;
	(*cub3d)->map.player_x = -1;
	(*cub3d)->map.player_y = -1;
	(*cub3d)->fd = -1;
	(*cub3d)->cube_file = NULL;
}

void	fetch_min_map_width(char **map, size_t *min_width)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (map[i])
	{
		j = 0;
		while (map[i][j])
			j++;
		if (j < *min_width)
			*min_width = j;
		i++;
	}
}

void	get_color(char *line, int *color, t_cub3d **cub3d_data)
{
	char	**split_colors;

	split_colors = ft_split(line, ' ');
	if (!split_colors)
	{
		free_2d_array(split_colors);
		err_exit("", cub3d_data);
	}
	else if (!split_colors[1])
	{
		free_2d_array(split_colors);
		err_exit("Error\nInvalid color\n", cub3d_data);
	}
	else if (split_colors[2])
	{
		free_2d_array(split_colors);
		err_exit("Error\nInvalid color\n", cub3d_data);
	}
	retrieve_color(split_colors, color, cub3d_data);
	free_2d_array(split_colors);
}

void	retrieve_color(char **split_colors, int *color, t_cub3d **cub3d_data)
{
	char	**split_rgb;
	int		r;
	int		g;
	int		b;

	split_rgb = ft_split(split_colors[1], ',');
	if (!split_rgb)
		free_arrays_exit(split_colors, split_rgb,
			"Error\nFailed to split colors\n", cub3d_data);
	else if (!split_rgb[0] || !split_rgb[1] || !split_rgb[2])
		free_arrays_exit(split_colors, split_rgb,
			"Error\nInvalid color\n", cub3d_data);
	if (split_rgb[3])
		free_arrays_exit(split_colors, split_rgb,
			"Error\nInvalid color\n", cub3d_data);
	r = ft_atoi_mod(split_rgb[0], cub3d_data, split_colors, split_rgb);
	g = ft_atoi_mod(split_rgb[1], cub3d_data, split_colors, split_rgb);
	b = ft_atoi_mod(split_rgb[2], cub3d_data, split_colors, split_rgb);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		free_arrays_exit(split_colors, split_rgb,
			"Error\nInvalid color\n", cub3d_data);
	*color = (r << 16) | (g << 8) | b;
	free_2d_array(split_rgb);
}
