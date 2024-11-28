/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:27:25 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:42:09 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int key_press(int keycode, t_data *data)
{

    if (!data)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_W)
        data->game.player.walkDirection = 1;
    else if (keycode == KEY_DOWN || keycode == KEY_S)
        data->game.player.walkDirection = -1;
    else if (keycode == KEY_LEFT || keycode == KEY_A)
        data->game.player.turnDirection = -1;
    else if (keycode == KEY_RIGHT || keycode == KEY_D)
        data->game.player.turnDirection = 1;
    else if (keycode == KEY_ESC)
        cleanup(data);
    else if (keycode == KEY_ACTIVE_MOUSSE)
        data->flage_mousse *= -1;
/*     else if(keycode == 32)
    {
        // clear the shotgun already loaded
        data->game.player.first_frame = false;
        render_player(data, 1);
        //mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
        //usleep(100000); // Sleep for 100 milliseconds
        render_player(data, 2);
        //mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
        //usleep(100000); // Sleep for 100 milliseconds
        render_player(data, 3);
        //mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
        //usleep(100000); // Sleep for 100 milliseconds
        render_player(data, 4);
        //mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
       
    } */
    return 0;
}

int key_release(int keycode, t_data *data)
{
    if (!data)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_DOWN || 
        keycode == KEY_W || keycode == KEY_S)
        data->game.player.walkDirection = 0;
    else if (keycode == KEY_LEFT || keycode == KEY_RIGHT || 
             keycode == KEY_A || keycode == KEY_D)
        data->game.player.turnDirection = 0;
    data->game.player.first_frame = true;
    return 0;
}

int mouse_move(int x, int y, t_data *data) 
{
    if (data->flage_mousse == -1)
        return 0;
    if (!data)
        return -1;

    static int first_move = 1;
    if (first_move) {
        data->game.last_mouse_x = x;
        first_move = 0;
        return 0;
    }

    int delta_x = x - data->game.last_mouse_x;
    
    data->game.player.rotationAngle += delta_x * MOUSE_SENSITIVITY * (M_PI / 180);
    data->game.player.rotationAngle = normalizeAngle(data->game.player.rotationAngle);
    
    data->game.last_mouse_x = x;
    
    return 0;
}