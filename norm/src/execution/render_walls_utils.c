/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:26:44 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/27 11:29:00 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>

int	get_wall_top(int wallHeight)
{
	return ((WINDOW_HEIGHT / 2) - (wallHeight / 2));
}

int	get_wall_bottom(int wallheight)
{
	int	wallbottom;

	wallbottom = (WINDOW_HEIGHT / 2) + (wallheight / 2);
	if (wallbottom >= WINDOW_HEIGHT)
		return (WINDOW_HEIGHT - 1);
	return (wallbottom);
}

int	get_wall_height(float perpDistance)
{
	return ((int)((WINDOW_HEIGHT / perpDistance) * TILE_SIZE));
}

int	get_texture_id(t_data *data, int i)
{
	double	rayangle;

	rayangle = normalizeangle(data->rays[i].rayangle);
	if (data->rays[i].wasHitVertical)
	{
		if (cos(rayangle) > 0)
			return (3);
		else
			return (2);
	}
	else
	{
		if (sin(rayangle) > 0)
			return (0);
		else
			return (1);
	}
}
