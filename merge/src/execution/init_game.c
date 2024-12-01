/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:06:03 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/01 15:20:29 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	set_player_orientation(t_player *player, char orientation)
{
	if (orientation == 'N')
		player->rotationangle = 3 * M_PI / 2;
	else if (orientation == 'S')
		player->rotationangle = M_PI / 2;
	else if (orientation == 'E')
		player->rotationangle = 0;
	else if (orientation == 'W')
		player->rotationangle = M_PI;
}

void	init_frames(t_data *data)
{
	size_t		i;
	t_player	*player;

	i = 0;
	player = &data->game.player;
	while (i < PLAYER_FRAMES)
	{
		update_frame(data);
		player->frames[i].img_ptr = mlx_xpm_file_to_image(data->mlx,
				player->frames[i].path, &player->player_w, &player->player_h);
		if (player->frames[i].img_ptr)
			player->frames[i].image_pixel_ptr
				= mlx_get_data_addr(player->frames[i].img_ptr,
					&player->frames[i].bits_per_pixel,
					&player->frames[i].line_len, &player->frames[i].endian);
		i++;
	}
}

void	init_player(t_data *data)
{
	data->game.player.turndirection = 0;
	data->game.player.walkdirection = 0;
	data->game.player.rotationangle = M_PI / 2;
	data->game.player.movespeed = 6.0;
	set_player_orientation(&data->game.player,
		data->game.player.player_direction);
	data->game.player.rotationspeed = 3 * (M_PI / 180);
	data->game.last_mouse_x = 0;
	data->game.player.sidedirection = 0;
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
