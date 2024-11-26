/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_cub_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achahid- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:01 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 08:10:02 by achahid-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/parsing.h"

void		load_cub3d_file(char *file, t_cub3d *cub3d);
static void	file_is_valid(char **cube_file, t_cub3d **cub3d);
static void	fetch_textures(char **cube_file, t_cub3d **cub3d);
static void	fetch_colors(char **cube_file, t_cub3d **cub3d);

void	load_cub3d_file(char *file, t_cub3d *cub3d)
{
	int	line_count;

	line_count = 0;
	init_cub3d_data(&cub3d, cub3d->fd);
	cub3d->fd = open(file, O_RDONLY);
	if (!cub3d->fd)
		err_exit("Error\nfile can not open\n", NULL);
	if (check_cub_extension(file) == false)
		err_exit("Error\nInvalid file extension\n", &cub3d);
	get_lines(&line_count, &cub3d);
	file_is_valid(cub3d->cube_file, &cub3d);
	load_cub3d_map(cub3d->cube_file, &cub3d);
	fetch_textures(cub3d->cube_file, &cub3d);
	fetch_colors(cub3d->cube_file, &cub3d);
	cub3d->player.x = cub3d->map.player_x * TILE_SIZE +(TILE_SIZE / 2);
	cub3d->player.y = cub3d->map.player_y * TILE_SIZE + (TILE_SIZE / 2);
	free_2d_array(cub3d->cube_file);
}

static void	file_is_valid(char **cube_file, t_cub3d **cub3d)
{
	size_t	i;

	i = 0;
	while (cube_file[i])
	{
		check_file_lines(cube_file, cub3d, i);
		i++;
	}
	if (all_cub3d_data_set(cub3d) == false)
		err_exit("Error\nMissing texture or color\n", cub3d);
}

static void	fetch_textures(char **cube_file, t_cub3d **cub3d_data)
{
	size_t	i;

	i = 0;
	while (cube_file[i])
	{
		if (!ft_strncmp("NO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.no.path,
				cub3d_data);
		else if (!ft_strncmp("SO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.so.path,
				cub3d_data);
		else if (!ft_strncmp("WE ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.we.path,
				cub3d_data);
		else if (!ft_strncmp("EA ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.ea.path,
				cub3d_data);
		i++;
	}
}

static void	fetch_colors(char **cube_file, t_cub3d **cub3d_data)
{
	size_t	i;

	i = 0;
	while (cube_file[i] != NULL)
	{
		if (!ft_strncmp("F ", cube_file[i], 2))
			get_color(cube_file[i], &(*cub3d_data)->colors.floor.color,
				cub3d_data);
		else if (!ft_strncmp("C ", cube_file[i], 2))
			get_color(cube_file[i], &(*cub3d_data)->colors.ceiling.color,
				cub3d_data);
		i++;
	}
}
