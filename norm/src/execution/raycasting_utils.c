/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:16:36 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/28 09:26:17 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	is_within_bounds(t_map *map, double x, double y)
{
	int	mapx;
	int	mapy;

	if (!map || !map->map || x < COLLISION_MARGIN || x >= (map->width
			* TILE_SIZE - COLLISION_MARGIN) || y < COLLISION_MARGIN
		|| y >= (map->height * TILE_SIZE - COLLISION_MARGIN))
		return (0);
	mapx = (int)(x / TILE_SIZE);
	mapy = (int)(y / TILE_SIZE);
	if (mapy >= map->height || mapx >= map->width || mapy < 0 || mapx < 0)
		return (0);
	return (1);
}

int	has_wall_at(t_map *map, double x, double y)
{
	if (!is_within_bounds(map, x, y))
		return (1);
	if (is_wall_at(map, x, y))
		return (1);
	return (0);
}

int	unit_circle(float angle, char c)
{
	if (c == 'x')
		return (angle > 0 && angle < M_PI);
	else if (c == 'y')
		return (angle > (M_PI / 2) && angle < (3 * M_PI) / 2);
	return (0);
}

int	inter_check(t_data *data, float angle, float *inter, float *step)
{
	if (data->is_horizon)
	{
		if (angle > 0 && angle < M_PI)
		{
			*inter += data->game.map.title_size;
			return (-1);
		}
		*step *= -1;
	}
	else
	{
		if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2))
		{
			*inter += data->game.map.title_size;
			return (-1);
		}
		*step *= -1;
	}
	return (1);
}

int	wall_hit(float x, float y, t_data *data)
{
	int	x_m;
	int	y_m;

	if (x < 0 || x >= data->game.map.width * TILE_SIZE || y < 0
		|| y >= data->game.map.height * TILE_SIZE)
		return (0);
	x_m = floor(x / data->game.map.title_size);
	y_m = floor(y / data->game.map.title_size);
	if (y_m >= data->game.map.height || x_m >= data->game.map.width)
		return (0);
	if (data->game.map.map[y_m]
		&& x_m < (int)ft_strlen(data->game.map.map[y_m]))
		if (data->game.map.map[y_m][x_m] == '1')
			return (0);
	return (1);
}
