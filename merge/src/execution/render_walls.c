/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:09:39 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/26 11:20:56 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	get_wall_top(int wallHeight)
{
	return ((WINDOW_HEIGHT / 2) - (wallHeight / 2));
}

int	get_wall_bottom(int wallheight)
{
	int	wallbottom;

	wallbottom = (WINDOW_HEIGHT / 2) + (wallheight / 2);
	if (wallbottom >= WINDOW_HEIGHT)
		return (WINDOW_HEIGHT - 1);
	return (wallbottom);
}

int	get_wall_height(float perpDistance)
{
	return (int)((WINDOW_HEIGHT / perpDistance) * TILE_SIZE);
}

int	get_texture_id(t_data *data, int i)
{
	double	rayAngle;

	rayAngle = normalizeAngle(data->rays[i].rayAngle);
	if (data->rays[i].wasHitVertical)
		return (cos(rayAngle) > 0 ? 3 : 2);
	else
		return (sin(rayAngle) > 0 ? 0 : 1);
}

t_wall	init_daya_walls(t_wall *wall, t_data *data, int i)
{
	wall->perpDistance = data->rays[i].distance * cos(data->rays[i].rayAngle
			- data->game.player.rotationAngle);
	wall->wallHeight = get_wall_height(wall->perpDistance);
	wall->wallTop = get_wall_top(wall->wallHeight);
	wall->wallBottom = get_wall_bottom(wall->wallHeight);
	wall->texture_id = get_texture_id(data, i);
	if (data->rays[i].wasHitVertical)
	{
		wall->wallX = fmod(data->game.player.y + data->rays[i].distance
				* sin(data->rays[i].rayAngle), data->game.map.title_size)
			/ data->game.map.title_size;
	}
	else
	{
		wall->wallX = fmod(data->game.player.x + data->rays[i].distance
				* cos(data->rays[i].rayAngle), data->game.map.title_size)
			/ data->game.map.title_size;
	}
}

void	render_walls(t_data *data)
{
	t_wall wall;

	for (int i = 0; i < NUM_RAYS; i++)
	{
		init_daya_walls(&wall, data, i);
		data->wall = &wall;

		for (int y = wall.wallTop; y <= wall.wallBottom; y++)
		{
			int texColor = my_mlx_pixel_get(wall.texture_id, data,
					data->wall->wallX, y - wall.wallTop, wall.wallHeight);

			double darkness = 1.0f + (wall.perpDistance / DARKNESS);

			int r = ((texColor >> 16) & 0xFF) / darkness;
			int g = ((texColor >> 8) & 0xFF) / darkness;
			int b = (texColor & 0xFF) / darkness;

			r = (r < 0) ? 0 : (r > 255) ? 255 : r;
			g = (g < 0) ? 0 : (g > 255) ? 255 : g;
			b = (b < 0) ? 0 : (b > 255) ? 255 : b;

			int finalColor = create_trgb(0, r, g, b);
			my_pixel_put(&data->img, i, y, finalColor);
		}
	}
}