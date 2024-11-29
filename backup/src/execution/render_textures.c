/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:11:51 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:42:38 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int create_trgb(int t, int r, int g, int b)
{
    return (t << 24) | (r << 16) | (g << 8) | b;
}
int my_mlx_pixel_get(int flag, t_data *data, double wallX, int y, int wallHeight) 
{
    t_dir_texture texter[5];
    
    if (!data || flag < 0 || flag > 4)
        return 0;
        
    texter[0] = data->game.textures.ea;
    texter[1] = data->game.textures.no;
    texter[2] = data->game.textures.so;
    texter[3] = data->game.textures.we;
    texter[4] = data->game.textures.ciel;

    if (flag == 4)
    {
        wallX = (int)wallX % 720;
        y = y % 720;

        char *pixel_ptr = texter[flag].image_pixel_ptr + 
                     ((int)wallX * texter[flag].line_len + 
                      y * (texter[flag].bits_per_pixel / 8));

        return *(unsigned int*)pixel_ptr;

    }

    int tex_x = (int)(wallX * texter[flag].texter_with);
    int tex_y = (int)((double)y * texter[flag].texter_height / wallHeight);

    tex_x = tex_x < 0 ? 0 : tex_x;
    tex_y = tex_y < 0 ? 0 : tex_y;
    tex_x = tex_x >= texter[flag].texter_with ? texter[flag].texter_with - 1 : tex_x;
    tex_y = tex_y >= texter[flag].texter_height ? texter[flag].texter_height - 1 : tex_y;

     if (!texter[flag].image_pixel_ptr ||  
          texter[flag].line_len <= 0)
         return 0;

    char *pixel_ptr = texter[flag].image_pixel_ptr + 
                     (tex_y * texter[flag].line_len + 
                      tex_x * (texter[flag].bits_per_pixel / 8));

    return *(unsigned int*)pixel_ptr;
}


double get_wallX(t_data *data, int i, float perpDistance)
{
    double rayAngle = normalizeAngle(data->rays[i].rayAngle);
    if (data->rays[i].wasHitVertical)
        return fmod(data->game.player.y + perpDistance * sin(rayAngle), TILE_SIZE) / TILE_SIZE;
    else
        return fmod(data->game.player.x + perpDistance * cos(rayAngle), TILE_SIZE) / TILE_SIZE;
}


void render_wall_texture(t_data *data, int i)
{
    int texColor;
    float darkness;
    double wallX = get_wallX(data, i, data->wall->perpDistance);

    for (int y = data->wall->wallTop; y <= data->wall->wallBottom; y++)
    {
        texColor = my_mlx_pixel_get(data->wall->texture_id, data, wallX, y - data->wall->wallTop, data->wall->wallHeight);
        darkness = 1.0f + (data->wall->perpDistance / DARKNESS);
        int finalColor = apply_darkness(texColor, darkness);
        my_pixel_put(&data->img, i, y, finalColor);
    }
}