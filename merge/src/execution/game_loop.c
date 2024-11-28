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

void    render_player(t_data *data)
{
    t_player *player = &data->game.player;
    int i = 0;
    int j = 0;
    int *addr;
    int *addr2;

    // Ensure the player frame image is loaded
    if (!player->frames.image_pixel_ptr)
    {
        fprintf(stderr, "Error: Player frame image not loaded\n");
        return;
    }
    // Calculate the starting position to center the weapon at the bottom of the screen
    int start_x = (WINDOW_WIDTH / 2) - (player->player_w / 2);
    int start_y = WINDOW_HEIGHT - player->player_h;

    while (i < player->player_h)
    {
        addr = (int *)(player->frames.image_pixel_ptr + (i * player->frames.line_len));
        addr2 = (int *)(data->img.image_pixel_ptr + ((i + start_y) * data->img.line_len));
        j = 0;
        while (j < player->player_w)
        {
            if (addr[j] != 0x29ff08 && (j + start_x) >= 0 && (j + start_x) < WINDOW_WIDTH && (i + start_y) >= 0 && (i + start_y) < WINDOW_HEIGHT)
            {
                addr2[j + start_x] = addr[j];
            }
            j++;

        }
    
        i++;
    }
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
   /*  if (data->game.player.first_frame == true)
    {
        puts("heu hani dakhl");
        render_player(data, 0);
    } */
   update_frame(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
    return 0;
}
