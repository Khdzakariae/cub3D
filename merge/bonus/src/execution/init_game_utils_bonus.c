/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 15:07:14 by zel-khad          #+#    #+#             */
/*   Updated: 2024/12/01 15:14:32 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d_bonus.h>

void	init_textures_3(t_data *data)
{
	t_texture	*texters;

	texters = &data->game.textures;
	texters->menu.img_ptr = mlx_xpm_file_to_image(data->mlx, MENU,
			&texters->menu.texter_with, &texters->menu.texter_height);
	if (texters->menu.img_ptr)
		texters->menu.image_pixel_ptr = mlx_get_data_addr(texters->menu.img_ptr,
				&texters->menu.bits_per_pixel, &texters->menu.line_len,
				&texters->menu.endian);
	texters->ciel.img_ptr = mlx_xpm_file_to_image(data->mlx, CIEL,
			&texters->ciel.texter_with, &texters->ciel.texter_height);
	if (texters->ciel.img_ptr)
		texters->ciel.image_pixel_ptr = mlx_get_data_addr(texters->ciel.img_ptr,
				&texters->ciel.bits_per_pixel, &texters->ciel.line_len,
				&texters->ciel.endian);
}

void	init_textures_2(t_data *data)
{
	t_texture	*texters;

	texters = &data->game.textures;
	texters->so.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->so.path,
			&texters->so.texter_with, &texters->so.texter_height);
	if (texters->so.img_ptr)
		texters->so.image_pixel_ptr = mlx_get_data_addr(texters->so.img_ptr,
				&texters->so.bits_per_pixel, &texters->so.line_len,
				&texters->so.endian);
	texters->we.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->we.path,
			&texters->we.texter_with, &texters->we.texter_height);
	if (texters->we.img_ptr)
		texters->we.image_pixel_ptr = mlx_get_data_addr(texters->we.img_ptr,
				&texters->we.bits_per_pixel, &texters->we.line_len,
				&texters->we.endian);
}

void	init_textures(t_data *data)
{
	t_texture	*texters;

	texters = &data->game.textures;
	texters->ea.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->ea.path,
			&texters->ea.texter_with, &texters->ea.texter_height);
	if (texters->ea.img_ptr)
		texters->ea.image_pixel_ptr = mlx_get_data_addr(texters->ea.img_ptr,
				&texters->ea.bits_per_pixel, &texters->ea.line_len,
				&texters->ea.endian);
	texters->no.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->no.path,
			&texters->no.texter_with, &texters->no.texter_height);
	if (texters->no.img_ptr)
		texters->no.image_pixel_ptr = mlx_get_data_addr(texters->no.img_ptr,
				&texters->no.bits_per_pixel, &texters->no.line_len,
				&texters->no.endian);
	init_textures_2(data);
	init_textures_3(data);
	if (!texters->ciel.img_ptr || !texters->ea.img_ptr || !texters->menu.img_ptr
		|| !texters->no.img_ptr || !texters->so.img_ptr || !texters->we.img_ptr)
	{
		printf("ERROR : FAILED TO GET TEXTURES");
		cleanup(data, 2);
	}
}
