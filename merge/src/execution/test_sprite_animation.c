/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:42:55 by zel-khad          #+#    #+#             */
/*   Updated: 2024/02/26 14:03:14 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3d.h>
#include <string.h>

long long	the_time(void)
{
	struct timeval	time_;

	gettimeofday(&time_, NULL);
	return (time_.tv_sec * 1000LL + time_.tv_usec / 1000);
}

bool	timing(void)
{
	static clock_t	start_time;
	clock_t			current_time;

	current_time = the_time();
	if (start_time == 0)
		return (start_time = current_time, true);
	else if ((current_time - start_time) <= 100)
		return (false);
	start_time = 0;
	return (true);
}

char	*get_assets(const int current_image)
{
	char	*assets;
	char	*number;

	assets = ft_calloc(20, sizeof(char));
	number = ft_itoa(current_image);
	strcat(assets, "player_frames/");
	strcat(assets, number);
	strcat(assets, ".xpm");
	free(number);
	return (assets);
}

void	display_frame(t_data *data, char *assets)
{
	int	images_size;

	data->game.player.frames.img_ptr = mlx_xpm_file_to_image(data->mlx, assets, &data->game.player.player_w, &data->game.player.player_h);
	data->game.player.frames.image_pixel_ptr = mlx_get_data_addr(data->game.player.frames.img_ptr, &data->game.player.frames.bits_per_pixel, &data->game.player.frames.line_len, &data->game.player.frames.endian);
	render_player(data);
	mlx_put_image_to_window(data->mlx, data->win, data->game.player.frames.img_ptr, 0, 0);
	/* mlx_destroy_image(data->mlx, &data->game.player.frames.img_ptr); */
}

int	update_frame(t_data *data)
{
	static int	n_img = 1;
	char		*assets;

	if (!timing())
		return (1);
	if (n_img == 29)
		n_img = 1;
	assets = get_assets(n_img++);
	display_frame(data, assets);
	free(assets);
	return (1);
}
