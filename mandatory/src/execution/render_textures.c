/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:11:51 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/30 20:54:51 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	create_trgb(int t, int r, int g, int b)
{
	return ((t << 24) | (r << 16) | (g << 8) | b);
}

void	init_tab(t_dir_texture *texter, t_data *data)
{
	texter[0] = data->game.textures.ea;
	texter[1] = data->game.textures.no;
	texter[2] = data->game.textures.so;
	texter[3] = data->game.textures.we;
	texter[4] = data->game.textures.menu;
}

int	my_mlx_pixel_get(int flag, t_data *data, double wallx, int y)
{
	t_dir_texture	texter[5];
	char			*pixel_ptr;
	int				tex_x;
	int				tex_y;
	int				wallheight;

	wallheight = data->wall->wallheight;
	if (!data || flag < 0 || flag > 4)
		return (0);
	init_tab(texter, data);
	tex_x = (int)(wallx * texter[flag].texter_with);
	tex_y = (int)((double)y * texter[flag].texter_height / wallheight);
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

double	get_wallx(t_data *data, int i, float perpdistance)
{
	double	rayangle;

	rayangle = normalizeangle(data->rays[i].rayangle);
	if (data->rays[i].washitvertical)
		return (fmod(data->game.player.y + perpdistance * sin(rayangle),
				TILE_SIZE) / TILE_SIZE);
	else
		return (fmod(data->game.player.x + perpdistance * cos(rayangle),
				TILE_SIZE) / TILE_SIZE);
}

void	render_wall_texture(t_data *data, int i)
{
	int		finalcolor;
	int		texcolor;
	float	darkness;
	double	wallx;
	int		y;

	wallx = get_wallx(data, i, data->wall->perpdistance);
	y = data->wall->walltop;
	while (y <= data->wall->wallbottom)
	{
		texcolor = my_mlx_pixel_get(data->wall->texture_id, data, wallx, y
				- data->wall->walltop);
		darkness = 1.0f + (data->wall->perpdistance / DARKNESS);
		finalcolor = apply_darkness(texcolor, darkness);
		my_pixel_put(&data->img, i, y, finalcolor);
		y++;
	}
}
