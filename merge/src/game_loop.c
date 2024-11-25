/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:26 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 19:31:22 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/cub3d.h"



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
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
    return 0;
}