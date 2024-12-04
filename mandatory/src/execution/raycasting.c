/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:13:52 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/29 19:28:09 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

float	get_h_inter(t_data *data, float angle)
{
	int		pixel;
	float	h_x;
	float	h_y;
	float	x_step;
	float	y_step;

	y_step = data->game.map.tile_size;
	x_step = data->game.map.tile_size / tan(angle);
	h_y = floor(data->game.player.y / data->game.map.tile_size)
		* data->game.map.tile_size;
	data->is_horizon = 1;
	pixel = inter_check(data, angle, &h_y, &y_step);
	h_x = data->game.player.x + (h_y - data->game.player.y) / tan(angle);
	if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y')
			&& x_step < 0))
		x_step *= -1;
	while (wall_hit(h_x, h_y - pixel, data))
	{
		h_x += x_step;
		h_y += y_step;
	}
	return (sqrt(pow(h_x - data->game.player.x, 2) + pow(h_y
				- data->game.player.y, 2)));
}

float	get_v_inter(t_data *data, float angle)
{
	int		pixel;
	float	v_x;
	float	v_y;
	float	x_step;
	float	y_step;

	x_step = data->game.map.tile_size;
	y_step = data->game.map.tile_size * tan(angle);
	v_x = floor(data->game.player.x / data->game.map.tile_size)
		* data->game.map.tile_size;
	data->is_horizon = 0;
	pixel = inter_check(data, angle, &v_x, &x_step);
	v_y = data->game.player.y + (v_x - data->game.player.x) * tan(angle);
	if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x')
			&& y_step > 0))
		y_step *= -1;
	while (wall_hit(v_x - pixel, v_y, data))
	{
		v_x += x_step;
		v_y += y_step;
	}
	return (sqrt(pow(v_x - data->game.player.x, 2) + pow(v_y
				- data->game.player.y, 2)));
}

void	castray(t_data *data, float rayangle, int stripId)
{
	float	h_inter;
	float	v_inter;

	rayangle = normalizeangle(rayangle);
	h_inter = get_h_inter(data, rayangle);
	v_inter = get_v_inter(data, rayangle);
	if (v_inter <= h_inter)
	{
		data->rays[stripId].distance = v_inter;
		data->rays[stripId].washitvertical = TRUE;
		data->rays[stripId].rayangle = rayangle;
	}
	else
	{
		data->rays[stripId].distance = h_inter;
		data->rays[stripId].washitvertical = FALSE;
		data->rays[stripId].rayangle = rayangle;
	}
}

void	castallrays(t_data *data)
{
	int		stripid;
	float	rayangle;

	stripid = 0;
	rayangle = data->game.player.rotationangle - (FOV_ANGLE / 2);
	while (stripid < NUM_RAYS)
	{
		castray(data, rayangle, stripid);
		rayangle += FOV_ANGLE / NUM_RAYS;
		stripid++;
	}
}
