/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:26 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/02 11:39:24 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

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

void	menu(t_data *data)
{
	t_texture	*player;
	int			i;
	int			j;
	int			*addr;
	int			*addr2;

	player = &data->game.textures;
	i = 0;
	j = 0;
	while (i < player->menu.texter_height)
	{
		addr = (int *)(player->menu.image_pixel_ptr + (i
					* player->menu.line_len));
		addr2 = (int *)(data->img.image_pixel_ptr + (i * data->img.line_len));
		j = 0;
		while (j < player->menu.texter_with)
		{
			if ((unsigned int)addr[j] != 0xFF000000 && j >= 0
				&& j < WINDOW_WIDTH && i >= 0 && i < WINDOW_HEIGHT)
				addr2[j] = addr[j];
			j++;
		}
		i++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

int	game_loop(t_data *data)
{
	if (data->flage_menue == 0)
		return (0);
	update_player(&data->game.player, &data->game.map);
	mlx_clear_window(data->mlx, data->win);
	ft_memset(data->img.image_pixel_ptr, 0, WINDOW_WIDTH * WINDOW_HEIGHT
		* (data->img.bits_per_pixel / 8));
	if (CIEL_IMAGE)
		render_ciel(data);
	else
		drawing_east(data);
	drawing_floor(data);
	castallrays(data);
	render_walls(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
	return (0);
}
