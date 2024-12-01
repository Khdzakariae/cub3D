/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_floor_ciel.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:09:53 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/01 15:03:56 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;

	if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
	{
		offset = (y * img->line_len) + (x * (img->bits_per_pixel / 8));
		*((unsigned int *)(img->image_pixel_ptr + offset)) = color;
	}
}

int	apply_darkness(int texcolor, float darkness)
{
	int	r;
	int	g;
	int	b;

	r = ((texcolor >> 16) & 0xFF) / darkness;
	g = ((texcolor >> 8) & 0xFF) / darkness;
	b = (texcolor & 0xFF) / darkness;
	if (r < 0)
		r = 0;
	else if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	else if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	else if (b > 255)
		b = 255;
	return (create_trgb(0, r, g, b));
}

void	render_ciel(t_data *data)
{
	t_texture	*player;
	int			i;
	int			j;
	int			*addr;
	int			*addr2;

	player = &data->game.textures;
	i = 0;
	j = 0;
	while (i < WINDOW_HEIGHT)
	{
		addr = ((int *)(player->ciel.image_pixel_ptr + (i
						* player->ciel.line_len)));
		addr2 = (int *)(data->img.image_pixel_ptr + (i * data->img.line_len));
		j = 0;
		while (j < WINDOW_WIDTH)
		{
			if (j >= 0 && (j) < WINDOW_WIDTH && i >= 0 && i < WINDOW_HEIGHT)
			{
				addr2[j] = addr[j];
			}
			j++;
		}
		i++;
	}
}

void	drawing_east(t_data *data)
{
	int	x;
	int	y;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		y = 0;
		while (y < WINDOW_HEIGHT / 2)
		{
			my_pixel_put(&data->img, x, y, data->game.colors.ceiling.color);
			y++;
		}
		x++;
	}
}

void	drawing_floor(t_data *data)
{
	int	x;
	int	y;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		y = WINDOW_HEIGHT / 2;
		while (y < WINDOW_HEIGHT)
		{
			my_pixel_put(&data->img, x, y, data->game.colors.floor.color);
			y++;
		}
		x++;
	}
}
