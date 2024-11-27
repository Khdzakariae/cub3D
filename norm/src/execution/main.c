/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:44:09 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/27 11:24:20 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	init(t_data *data)
{
	init_game(data);
	if (!data->mlx || !data->win || !data->img.img_ptr)
		return (false);
	init_player(data);
	init_textures(data);
	data->flage_mousse = -1;
	return (true);
}

int	main(int argc, char **argv)
{
	t_cub3d	cub3d;
	t_data	data;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
		return (1);
	}
	ft_memset(&data, 0, sizeof(t_data));
	load_cub3d_file(argv[1], &cub3d);
	data.game = cub3d;
	if (!init(&data))
	{
		fprintf(stderr, "Error: Failed to initialize game\n");
		cleanup(&data);
		return (1);
	}
	mlx_loop_hook(data.mlx, game_loop, &data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 6, 1L << 6, mouse_move, &data);
	mlx_hook(data.win, 17, 0, cleanup, &data);
	mlx_loop(data.mlx);
	return (0);
}
