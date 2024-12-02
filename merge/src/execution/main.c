/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:44:09 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/01 20:52:10 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

bool	init(t_data *data)
{
	init_game(data);
	if (!data->mlx || !data->win || !data->img.img_ptr)
		return (false);
	init_player(data);
	init_frames(data);
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
		ft_putstr_fd("Usage: ./cub3D <map_file>\n",2 );
		return (1);
	}
	ft_memset(&data, 0, sizeof(t_data));
	load_cub3d_file(argv[1], &cub3d);
	data.game = cub3d;
	if (!init(&data))
	{
		ft_putstr_fd("Error: Failed to initialize game\n", 2);
		cleanup(&data, 2);
		return (1);
	}
	menu(&data);
	sleep(2);
	mlx_loop_hook(data.mlx, game_loop, &data);
	mlx_hook(data.win, 2, 1L << 0, key_press, &data);
	mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	mlx_hook(data.win, 6, 1L << 6, mouse_move, &data);
	mlx_loop(data.mlx);
	return (0);
}
