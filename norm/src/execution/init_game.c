/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:03 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/26 17:38:56 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	set_player_orientation(t_player *player, char orientation)
{
	if (orientation == 'N')
		player->rotationAngle = 3 * M_PI / 2;
	else if (orientation == 'S')
		player->rotationAngle = M_PI / 2;
	else if (orientation == 'E')
		player->rotationAngle = 0;
	else if (orientation == 'W')
		player->rotationAngle = M_PI;
}

void	init_player(t_data *data)
{
	data->game.player.radius = 3;
	data->game.player.turnDirection = 0;
	data->game.player.walkDirection = 0;
	data->game.player.rotationAngle = M_PI / 2;
	data->game.player.moveSpeed = 6.0;
	set_player_orientation(&data->game.player,
		data->game.player.player_direction);
	data->game.player.rotationSpeed = 3 * (M_PI / 180);
}

void	init_textures_2(t_data *data)
{
	t_texture	*texters;

	texters = &data->game.textures;
	texters->so.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->so.path,
			&texters->so.texter_with, &texters->so.texter_height);
	if (texters->so.img_ptr)
		texters->so.image_pixel_ptr = mlx_get_data_addr(texters->so.img_ptr,
				&texters->so.bits_per_pixel, &texters->so.line_len,
				&texters->so.endian);
	texters->we.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->we.path,
			&texters->we.texter_with, &texters->we.texter_height);
	if (texters->we.img_ptr)
		texters->we.image_pixel_ptr = mlx_get_data_addr(texters->we.img_ptr,
				&texters->we.bits_per_pixel, &texters->we.line_len,
				&texters->we.endian);
	texters->ciel.img_ptr = mlx_xpm_file_to_image(data->mlx, CIEL,
			&texters->ciel.texter_with, &texters->ciel.texter_height);
	if (texters->ciel.img_ptr)
		texters->ciel.image_pixel_ptr = mlx_get_data_addr(texters->ciel.img_ptr,
				&texters->ciel.bits_per_pixel, &texters->ciel.line_len,
				&texters->ciel.endian);
}

void	init_textures(t_data *data)
{
	t_texture	*texters;

	texters = &data->game.textures;
	texters->ea.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->ea.path,
			&texters->ea.texter_with, &texters->ea.texter_height);
	if (texters->ea.img_ptr)
		texters->ea.image_pixel_ptr = mlx_get_data_addr(texters->ea.img_ptr,
				&texters->ea.bits_per_pixel, &texters->ea.line_len,
				&texters->ea.endian);
	texters->no.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->no.path,
			&texters->no.texter_with, &texters->no.texter_height);
	if (texters->no.img_ptr)
		texters->no.image_pixel_ptr = mlx_get_data_addr(texters->no.img_ptr,
				&texters->no.bits_per_pixel, &texters->no.line_len,
				&texters->no.endian);
	init_textures_2(data);
}

void	init_game(t_data *data)
{
	if (!data)
		return ;
	data->mlx = mlx_init();
	if (!data->mlx)
		return ;
	data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
	if (!data->win)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		return ;
	}
	data->img.img_ptr = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!data->img.img_ptr)
	{
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		return ;
	}
	data->img.image_pixel_ptr = mlx_get_data_addr(data->img.img_ptr,
			&data->img.bits_per_pixel, &data->img.line_len, &data->img.endian);
}

bool	init(t_data *data)
{
	init_game(data);
	if (!data->mlx || !data->win || !data->img.img_ptr)
		return (false);
	init_player(data);
	init_textures(data);
	data->flage_mousse = -1;
	return (true);
}
