/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:09:39 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:42:43 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int get_wall_top(int wallHeight)
{
    return (WINDOW_HEIGHT / 2) - (wallHeight / 2);
}

int get_wall_bottom(int wallHeight)
{
    int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
    return wallBottom >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : wallBottom;
}


int get_wall_height(float perpDistance)
{
    return (int)((WINDOW_HEIGHT / perpDistance) * TILE_SIZE); 
}

int get_texture_id(t_data *data, int i)
{
    double rayAngle = normalizeAngle(data->rays[i].rayAngle);
    if (data->rays[i].wasHitVertical)
        return cos(rayAngle) > 0 ? 3 : 2;
    else
        return sin(rayAngle) > 0 ? 0 : 1;
}

void render_walls(t_data *data)
{
    t_wall wall;

    for (int i = 0; i < NUM_RAYS; i++)
    {
        wall.perpDistance = data->rays[i].distance *
                             cos(data->rays[i].rayAngle - data->game.player.rotationAngle);
        wall.wallHeight  = get_wall_height(wall.perpDistance);
        wall.wallTop = get_wall_top(wall.wallHeight);
        wall.wallBottom = get_wall_bottom(wall.wallHeight);

        wall.texture_id = get_texture_id(data, i);

        data->wall = &wall;
        render_wall_texture(data, i);
    }
}