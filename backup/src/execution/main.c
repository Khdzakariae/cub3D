/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:44:09 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/25 21:40:14 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;
    t_cub3d cub3d;
    ft_memset(&data, 0, sizeof(t_data));
    load_cub3d_file(argv[1], &cub3d);
    data.game = cub3d;

    if (!init(&data))
    {
        fprintf(stderr, "Error: Failed to initialize game\n");
        cleanup(&data);
        return 1;
    }
    mlx_loop_hook(data.mlx, game_loop, &data);
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_hook(data.win, 6, 1L<<6, mouse_move, &data);
    mlx_hook(data.win, 17, 0, cleanup, &data);
    mlx_loop(data.mlx);
    return 0;
}