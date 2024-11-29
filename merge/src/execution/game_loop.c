/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:26 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/28 11:07:02 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_movement(t_player *player, double *movestep, double *newplayerx,
		double *newplayery)
{
	player->rotationangle = normalizeangle(player->rotationangle
			+ player->turndirection * player->rotationspeed);
	*(movestep) = player->walkdirection * player->movespeed;
	*(newplayerx) = player->x + cos(player->rotationangle) * *(movestep);
	*(newplayery) = player->y + sin(player->rotationangle) * *(movestep);
}

int	update_player(t_player *player, t_map *map)
{
	double	movestep;
	double	newplayerx;
	double	newplayery;

	if (!player || !map)
		return (-1);
	init_movement(player, &movestep, &newplayerx, &newplayery);
	if (has_wall_at(map, newplayerx, player->y))
	{
		if (!has_wall_at(map, player->x, newplayery))
			player->y = newplayery;
	}
	else
		player->x = newplayerx;
	if (has_wall_at(map, player->x, newplayery))
	{
		if (!has_wall_at(map, newplayerx, player->y))
			player->x = newplayerx;
	}
	else
		player->y = newplayery;
	return (0);
}

int	game_loop(t_data *data)
{
	update_player(&data->game.player, &data->game.map);
	mlx_clear_window(data->mlx, data->win);
	ft_memset(data->img.image_pixel_ptr, 0, WINDOW_WIDTH * WINDOW_HEIGHT
		* (data->img.bits_per_pixel / 8));
	if (CIEL_IMAGE)
		ciel(data);
	else
		drawing_east(data);
	drawing_floor(data);
	castallrays(data);
	render_walls(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
	return (0);
}
