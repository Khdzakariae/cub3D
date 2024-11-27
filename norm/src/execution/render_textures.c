/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:11:51 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/27 11:36:40 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	create_trgb(int t, int r, int g, int b)
{
	return ((t << 24) | (r << 16) | (g << 8) | b);
}

int	my_mlx_pixel_get(int flag, t_data *data, double wallx, int y,
		int wallHeight)
{
	t_dir_texture	texter[5];
	char			*pixel_ptr;
	int				tex_x;
	int				tex_y;

	if (!data || flag < 0 || flag > 4)
		return (0);
	texter[0] = data->game.textures.ea;
	texter[1] = data->game.textures.no;
	texter[2] = data->game.textures.so;
	texter[3] = data->game.textures.we;
	texter[4] = data->game.textures.ciel;
	if (flag == 4)
	{
		wallx = (int)wallx % 720;
		y = y % 720;
		pixel_ptr = texter[flag].image_pixel_ptr + ((int)wallx
				* texter[flag].line_len + y * (texter[flag].bits_per_pixel
					/ 8));
		return (*(unsigned int *)pixel_ptr);
	}
	tex_x = (int)(wallx * texter[flag].texter_with);
	tex_y = (int)((double)y * texter[flag].texter_height / wallHeight);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_y < 0)
		tex_y = 0;
	if (tex_x >= texter[flag].texter_with)
		tex_x = texter[flag].texter_with - 1;
	if (tex_y >= texter[flag].texter_height)
		tex_y = texter[flag].texter_height - 1;
	if (!texter[flag].image_pixel_ptr || texter[flag].line_len <= 0)
		return (0);
	pixel_ptr = texter[flag].image_pixel_ptr + (tex_y * texter[flag].line_len
			+ tex_x * (texter[flag].bits_per_pixel / 8));
	return (*(unsigned int *)pixel_ptr);
}

double	get_wallx(t_data *data, int i, float perpDistance)
{
	double	rayangle;

	rayangle = normalizeangle(data->rays[i].rayangle);
	if (data->rays[i].wasHitVertical)
		return (fmod(data->game.player.y + perpDistance * sin(rayangle),
				TILE_SIZE) / TILE_SIZE);
	else
		return (fmod(data->game.player.x + perpDistance * cos(rayangle),
				TILE_SIZE) / TILE_SIZE);
}

void	render_wall_texture(t_data *data, int i)
{
	int		finalcolor;
	int		texcolor;
	float	darkness;
	double	wallx;
	int		y;

	wallx = get_wallx(data, i, data->wall->perpDistance);
	y = data->wall->wallTop;
	while (y <= data->wall->wallBottom)
	{
		texcolor = my_mlx_pixel_get(data->wall->texture_id, data, wallx, y
				- data->wall->wallTop, data->wall->wallHeight);
		darkness = 1.0f + (data->wall->perpDistance / DARKNESS);
		finalcolor = apply_darkness(texcolor, darkness);
		my_pixel_put(&data->img, i, y, finalcolor);
		y++;
	}
}
