/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:13:52 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:42:30 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

float get_h_inter(t_data *data, float angle)
{
    float h_x, h_y, x_step, y_step;
    int pixel;

    y_step = data->game.map.title_size;
    x_step = data->game.map.title_size / tan(angle);
    h_y = floor(data->game.player.y / data->game.map.title_size) * data->game.map.title_size;
    pixel = inter_check(data, angle, &h_y, &y_step, 1);
    h_x = data->game.player.x + (h_y - data->game.player.y) / tan(angle);

    if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y') && x_step < 0))
        x_step *= -1;

    while (wall_hit(h_x, h_y - pixel, data))
    {
        h_x += x_step;
        h_y += y_step;
    }

    return sqrt(pow(h_x - data->game.player.x, 2) + pow(h_y - data->game.player.y, 2));
}

float get_v_inter(t_data *data, float angle)
{
    float v_x, v_y, x_step, y_step;
    int pixel;

    x_step = data->game.map.title_size;
    y_step = data->game.map.title_size * tan(angle);
    v_x = floor(data->game.player.x / data->game.map.title_size) * data->game.map.title_size;
    pixel = inter_check(data, angle, &v_x, &x_step, 0);
    v_y = data->game.player.y + (v_x - data->game.player.x) * tan(angle);

    if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x') && y_step > 0))
        y_step *= -1;

    while (wall_hit(v_x - pixel, v_y, data))
    {
        v_x += x_step;
        v_y += y_step;
    }

    return sqrt(pow(v_x - data->game.player.x, 2) + pow(v_y - data->game.player.y, 2));
}

void castRay(t_data *data, float rayAngle, int stripId)
{
    float h_inter, v_inter;
    rayAngle = normalizeAngle(rayAngle);

    h_inter = get_h_inter(data, rayAngle);
    v_inter = get_v_inter(data, rayAngle);

    if (v_inter <= h_inter)
    {
        data->rays[stripId].distance = v_inter;
        data->rays[stripId].wasHitVertical = TRUE;
        data->rays[stripId].rayAngle = rayAngle;
    }
    else
    {
        data->rays[stripId].distance = h_inter;
        data->rays[stripId].wasHitVertical = FALSE;
        data->rays[stripId].rayAngle = rayAngle;
    }
}

void castAllRays(t_data *data)
{
    float rayAngle = data->game.player.rotationAngle - (FOV_ANGLE / 2);
    
    for (int stripId = 0; stripId < NUM_RAYS; stripId++)
    {
        castRay(data, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}