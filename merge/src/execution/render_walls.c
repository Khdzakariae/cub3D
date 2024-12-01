/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:09:39 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/28 11:06:25 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_daya_walls(t_wall *wall, t_data *data, int i)
{
	wall->perpdistance = data->rays[i].distance * cos(data->rays[i].rayangle
			- data->game.player.rotationangle);
	wall->wallheight = get_wall_height(wall->perpdistance);
	wall->walltop = get_wall_top(wall->wallheight);
	wall->wallbottom = get_wall_bottom(wall->wallheight);
	wall->texture_id = get_texture_id(data, i);
	if (data->rays[i].washitvertical)
	{
		wall->wallx = fmod(data->game.player.y + data->rays[i].distance
				* sin(data->rays[i].rayangle), data->game.map.tile_size)
			/ data->game.map.tile_size;
	}
	else
	{
		wall->wallx = fmod(data->game.player.x + data->rays[i].distance
				* cos(data->rays[i].rayangle), data->game.map.tile_size)
			/ data->game.map.tile_size;
	}
}

void	calculate_wall_color(int tex_color, double darkness, int *r, int *g)
{
	*r = ((tex_color >> 16) & 0xFF) / darkness;
	*g = ((tex_color >> 8) & 0xFF) / darkness;
	if (*r < 0)
		*r = 0;
	else if (*r > 255)
		*r = 255;
	if (*g < 0)
		*g = 0;
	else if (*g > 255)
		*g = 255;
}

void	calculate_blue_component(int tex_color, double darkness, int *b)
{
	*b = (tex_color & 0xFF) / darkness;
	if (*b < 0)
		*b = 0;
	else if (*b > 255)
		*b = 255;
}

void	render_vertical_wall_slice(t_data *data, t_wall *wall, int ray_x)
{
	int		y;
	int		r;
	int		g;
	int		b;

	y = wall->walltop;
	while (y <= wall->wallbottom)
	{
		wall->texcolor = my_mlx_pixel_get(wall->texture_id, data, wall->wallx, y
				- wall->walltop);
		wall->darkness = 1.0f + (wall->perpdistance / DARKNESS);
		calculate_wall_color(wall->texcolor, wall->darkness, &r, &g);
		calculate_blue_component(wall->texcolor, wall->darkness, &b);
		wall->finalcolor = create_trgb(0, r, g, b);
		my_pixel_put(&data->img, ray_x, y, wall->finalcolor);
		y++;
	}
}

void	render_walls(t_data *data)
{
	t_wall	wall;
	int		i;

	i = 0;
	while (i < NUM_RAYS)
	{
		init_daya_walls(&wall, data, i);
		data->wall = &wall;
		render_vertical_wall_slice(data, &wall, i);
		i++;
	}
}
