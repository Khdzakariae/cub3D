/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:20:30 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/26 19:13:43 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double	normalizeangle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

int	is_wall_at(t_map *map, double x, double y)
{
	int	mapx;
	int	mapy;
	int	radius;
	int checkx;
	int checky;
	int i;

	mapx = (int)(x / TILE_SIZE);
	mapy = (int)(y / TILE_SIZE);
	if (map->map[mapy][mapx] == '1')
		return (1);
	radius = RADIUS;
	int points[4][2] = {{-radius, -radius}, {-radius, radius}, {radius,
		-radius}, {radius, radius}};
	i = 0;
	while (i < 4)
	{
		checkx = (int)((x + points[i][0]) / TILE_SIZE);
		checky = (int)((y + points[i][1]) / TILE_SIZE);

		if (checkx >= 0 && checkx < map->width && checky >= 0
			&& checky < map->height && map->map[checky][checkx] == '1')
			return (1);
		 i++;
	}
	return (0);
}
