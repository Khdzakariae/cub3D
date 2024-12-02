/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 19:28:59 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/01 09:47:21 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d_bonus.h>

void	cleanup_textures(t_cub3d *game)
{
	free_2d_array(game->map.map);
	free(game->textures.ea.path);
	free(game->textures.we.path);
	free(game->textures.no.path);
	free(game->textures.so.path);
}

void	clean_frames(t_data *data)
{
	t_player	*player;
	size_t		i;

	player = &data->game.player;
	i = 0;
	while (i < PLAYER_FRAMES)
	{
		if (player->frames[i].img_ptr)
			mlx_destroy_image(data->mlx, player->frames[i].img_ptr);
		free(player->frames[i].path);
		i++;
	}
}

void	destroy_window(t_data *data, int flage)
{
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(flage);
}

void	cleanup(t_data *data, int flage)
{
	t_texture	*texters;

	texters = &data->game.textures;
	if (texters->ea.img_ptr)
		mlx_destroy_image(data->mlx, texters->ea.img_ptr);
	if (texters->no.img_ptr)
		mlx_destroy_image(data->mlx, texters->no.img_ptr);
	if (texters->so.img_ptr)
		mlx_destroy_image(data->mlx, texters->so.img_ptr);
	if (texters->we.img_ptr)
		mlx_destroy_image(data->mlx, texters->we.img_ptr);
	if (texters->menu.img_ptr)
		mlx_destroy_image(data->mlx, texters->menu.img_ptr);
	if (texters->ciel.img_ptr)
		mlx_destroy_image(data->mlx, texters->ciel.img_ptr);
	if (data->img.img_ptr)
		mlx_destroy_image(data->mlx, data->img.img_ptr);
	cleanup_textures(&data->game);
	clean_frames(data);
	destroy_window(data, flage);
}
