/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:20:30 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:42:18 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

double distanceBetweenPoints(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle;
}

int is_wall_at(t_map *map, double x, double y) 
{
    int mapX ;
    int mapY ;
    int radius;

    mapX= (int)(x / TILE_SIZE);
    mapY= (int)(y / TILE_SIZE);    
    if (map->map[mapY][mapX] == '1')
        return 1;
    radius = RADIUS;
    int points[4][2] = {
        {-radius, -radius},
        {-radius, radius},
        {radius, -radius},
        {radius, radius}
    };
    for (int i = 0; i < 4; i++) {
        int checkX = (int)((x + points[i][0]) / TILE_SIZE);
        int checkY = (int)((y + points[i][1]) / TILE_SIZE);
        
        if (checkX >= 0 && checkX < map->width && 
            checkY >= 0 && checkY < map->height && 
            map->map[checkY][checkX] == '1')
            return 1; 
    }
    return 0; 
}