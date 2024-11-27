/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:26 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:41:57 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int update_player(t_player *player, t_map *map)
{
    if (!player || !map)
        return -1;

    double moveStep;
    double newPlayerX, newPlayerY;

    player->rotationAngle = normalizeAngle(player->rotationAngle + 
                           player->turnDirection * player->rotationSpeed);

    moveStep = player->walkDirection * player->moveSpeed;
    newPlayerX = player->x + cos(player->rotationAngle) * moveStep;
    newPlayerY = player->y + sin(player->rotationAngle) * moveStep;

    if (has_wall_at(map, newPlayerX, player->y)) 
    {
        if (!has_wall_at(map, player->x, newPlayerY)) 
            player->y = newPlayerY; 
    } 
    else 
        player->x = newPlayerX; 
    if (has_wall_at(map, player->x, newPlayerY)) 
    {
        if (!has_wall_at(map, newPlayerX, player->y))
            player->x = newPlayerX; 
    } 
    else 
        player->y = newPlayerY; 
    return 0;
}

void    minimap_drawer(t_data *data, size_t x, size_t y, int color)
{
    size_t  i;
    size_t  j;
    int     minimap_tile_size;

    i = 0;
    minimap_tile_size = data->game.map.title_size * MINIMAP_SCALE_FACTOR;
    while (i < minimap_tile_size)
    {
        j = 0;
        while (j < minimap_tile_size)
        {
            my_pixel_put(&data->img, x * minimap_tile_size + j, y * minimap_tile_size + i, color);
            j++;
        }
        i++;
    }
}

void    minimap_draw_player(t_data *data)
{
    int player_x;
    int player_y;
    int player_size;
    int  i;
    int  j;

    player_x = data->game.player.x * MINIMAP_SCALE_FACTOR;
    player_y = data->game.player.y * MINIMAP_SCALE_FACTOR;
    player_size = 2;
    i = -player_size;
    while (i <= player_size)
    {
        j = player_size * -1;
        while (j <= player_size)
        {
            my_pixel_put(&data->img, player_x + j, player_y + i, MINIMAP_PLAYER_COLOR);
            j++;
        }
        i++;
    }
}

void draw_minimap(t_data *data)
{
    size_t  y;
    size_t  x;
    int     color;

    y = 0;
    while (y < data->game.map.height)
    {
        x = 0;
        while (x < data->game.map.width)
        {
            color = data->game.map.map[y][x] == '1' ? MINIMAP_WALL_COLOR : MINIMAP_EMPTY_COLOR;
            minimap_drawer(data, x, y, color);
            x++;
        }
        y++;
    }
    minimap_draw_player(data);
}

int game_loop(t_data *data)
{
    update_player(&data->game.player, &data->game.map);
    mlx_clear_window(data->mlx, data->win);
    
    ft_memset(data->img.image_pixel_ptr, 0, 
             WINDOW_WIDTH * WINDOW_HEIGHT * (data->img.bits_per_pixel / 8));
             
    if(CIEL_IMAGE)
        ciel(data);
    else
        drawing_east(data);
    drawing_floor(data);
    castAllRays(data);
    render_walls(data);
    draw_minimap(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
    return 0;
}
