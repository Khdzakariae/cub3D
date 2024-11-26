/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_cub3d_map.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achahid- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:31 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 08:10:33 by achahid-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/parsing.h"

void		load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void		fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
static void	copy_map_content(char **cube_file, t_cub3d **cub3d_data,
				size_t map_starting_i);
static void	move_content_to_map(char **cube_file, t_cub3d **cub3d_data,
				size_t map_starting_i, size_t map_i);

void	load_cub3d_map(char **cube_file, t_cub3d **cub3d_data)
{
	t_bool	map;
	size_t	i;

	map = false;
	i = 0;
	while (cube_file[i] != NULL)
	{
		if (cube_file[i][0] == '1' && map == false)
		{
			map = true;
			fill_map(cube_file, cub3d_data, &i);
			map_is_valid(cub3d_data);
			if (cube_file[i] == NULL)
				break ;
		}
		else if (cube_file[i][0] == '1' && map == true)
			err_exit("Error\nOnly one map is valid\n", cub3d_data);
		i++;
	}
	if ((*cub3d_data)->map.width == 0 || (*cub3d_data)->map.height == 0)
		err_exit("Error\nNo map found\n", cub3d_data);
}

void	fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i)
{
	size_t	map_starting_i;
	size_t	map_height;
	size_t	map_width;

	map_starting_i = *i;
	get_map_dimensions(cube_file, map_starting_i, &map_height, &map_width);
	*i += map_height;
	(*cub3d_data)->map.height = map_height;
	(*cub3d_data)->map.width = map_width;
	(*cub3d_data)->map.map = (char **)malloc(sizeof(char *) * (map_height + 1));
	if (!(*cub3d_data)->map.map)
		err_exit("Error\nFailed to allocte memory\n", cub3d_data);
	copy_map_content(cube_file, cub3d_data, map_starting_i);
}

static void	copy_map_content(char **cube_file,
				t_cub3d **cub3d_data, size_t map_starting_i)
{
	size_t	map_i;
	size_t	map_height_scaled;

	map_i = 0;
	map_height_scaled = (*cub3d_data)->map.height + map_starting_i;
	while (map_starting_i < map_height_scaled)
	{
		(*cub3d_data)->map.map[map_i]
			= (char *)malloc(sizeof(char) * (*cub3d_data)->map.width + 1);
		if ((*cub3d_data)->map.map[map_i] == NULL)
			free_incomplete_map(cub3d_data, map_i,
				"Error\nMap[i] allocation failed\n");
		move_content_to_map(cube_file, cub3d_data, map_starting_i, map_i);
		map_i++;
		map_starting_i++;
	}
	(*cub3d_data)->map.map[map_i] = NULL;
}

static void	move_content_to_map(char **cube_file, t_cub3d **cub3d_data,
			size_t map_starting_i, size_t map_i)
{
	size_t	j;
	size_t	map_j;

	j = 0;
	map_j = 0;
	while (cube_file[map_starting_i][j] != '\0')
	{
		if (valid_map_char(cube_file[map_starting_i][j]) == false)
			free_incomplete_map(cub3d_data, map_i,
				"Error\nInvalid Character\n");
		if (ft_isspace(cube_file[map_starting_i][j]) == false)
		{
			(*cub3d_data)->map.map[map_i][map_j] = cube_file[map_starting_i][j];
			map_j++;
		}
		j++;
	}
	while ((int)map_j < (*cub3d_data)->map.width)
	{
		(*cub3d_data)->map.map[map_i][map_j] = '\0';
		map_j++;
	}
	(*cub3d_data)->map.map[map_i][map_j] = '\0';
}
