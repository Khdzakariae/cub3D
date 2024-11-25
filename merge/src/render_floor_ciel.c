/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_floor_ciel.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:09:53 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 19:50:09 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cub3d.h"

void my_pixel_put(t_img *img, int x, int y, int color)
{
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        int offset = (y * img->line_len) + (x * (img->bits_per_pixel / 8));
        *((unsigned int *)(img->image_pixel_ptr + offset)) = color;
    }
}

int apply_darkness(int texColor, float darkness)
{
    int r = ((texColor >> 16) & 0xFF) / darkness;
    int g = ((texColor >> 8) & 0xFF) / darkness;
    int b = (texColor & 0xFF) / darkness;

    r = (r < 0) ? 0 : (r > 255) ? 255 : r;
    g = (g < 0) ? 0 : (g > 255) ? 255 : g;
    b = (b < 0) ? 0 : (b > 255) ? 255 : b;

    return create_trgb(0, r, g, b);
}

void ciel(t_data *data)
{
    int texColor;
    t_texture *texters = &data->game.textures;

    for (int x = 20; x < WINDOW_WIDTH; x++)
    {
        for (int y = 20; y < WINDOW_HEIGHT / 2; y++)
        {
            texColor = my_mlx_pixel_get(4, data, x, y , 0);
            my_pixel_put(&data->img, x, y, texColor);
        }
    }

}
void drawing_east(t_data *data)
{
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        for (int y = 0; y < WINDOW_HEIGHT / 2; y++)
        {
            my_pixel_put(&data->img, x, y, data->game.colors.ceiling.color);
        }
    }
}

void drawing_floor(t_data *data)
{
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y++)
        {
            my_pixel_put(&data->img, x, y, data->game.colors.floor.color);
        }
    }
}