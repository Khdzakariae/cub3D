/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:30:26 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/28 11:07:02 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

void	init_movement(t_player *player, double *movestep, double *newplayerx,
		double *newplayery)
{
	player->rotationangle = normalizeangle(player->rotationangle
			+ player->turndirection * player->rotationspeed);
	*(movestep) = player->walkdirection * player->movespeed;
	*(newplayerx) = player->x + cos(player->rotationangle) * *(movestep);
	*(newplayery) = player->y + sin(player->rotationangle) * *(movestep);
}

void    render_player(t_data *data, int frame)
{
    t_player *player = &data->game.player;
    int i = 0;
    int j = 0;
    int *addr;
    int *addr2;

    if (!player->frames[frame].image_pixel_ptr)
    {
        fprintf(stderr, "Error: Player frame image not loaded\n");
        return;
    }
    int start_x = (WINDOW_WIDTH / 2) - (player->player_w / 2);
    int start_y = WINDOW_HEIGHT - player->player_h;

    while (i < player->player_h)
    {
        addr = (int *)(player->frames[frame].image_pixel_ptr + (i * player->frames[frame].line_len));
        addr2 = (int *)(data->img.image_pixel_ptr + ((i + start_y) * data->img.line_len));
        j = 0;
        while (j < player->player_w)
        {
            if (addr[j] != 0xFF000000 && (j + start_x) >= 0 && (j + start_x) < WINDOW_WIDTH && (i + start_y) >= 0 && (i + start_y) < WINDOW_HEIGHT)
            {
                addr2[j + start_x] = addr[j];
            }
            j++;
        }
        i++;
    }
}

void    draw_player(t_data *data)
{
    static int i = 0;
    if (timing() == true)
    {
        render_player(data, i);
        i++;
    }
    else
    {
        if (i != 0)
            render_player(data, i - 1);
        else
            render_player(data, i);
    }
    
    if (i == 30)
        i = 0;
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
    draw_player(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
    return 0;
}
