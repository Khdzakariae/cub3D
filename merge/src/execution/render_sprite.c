/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:42:55 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/29 19:31:56 by zel-khad         ###   ########.fr       */
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
	else if ((current_time - start_time) <= 120)
		return (false);
	start_time = 0;
	return (true);
}

char	*get_assets(int current_image)
{
	char	*assets;
	char	*number;

	current_image += 1;
	assets = ft_calloc(30, sizeof(char));
	number = ft_itoa(current_image);
	strcat(assets, "sprite_frames/");
	strcat(assets, number);
	strcat(assets, ".xpm");
	free(number);
	return (assets);
}

int	update_frame(t_data *data)
{
	static int	n_img;
	char		*assets;

	if (n_img == PLAYER_FRAMES)
		n_img = 0;
	assets = get_assets(n_img);
	data->game.player.frames[n_img++].path = assets;
	return (1);
}

void	render_player_helper(t_data *data, int frame, int start_x, int start_y)
{
	int			i;
	int			j;
	int			*addr;
	int			*addr2;
	t_player	*player;

	i = 0;
	player = &data->game.player;
	while (i < player->player_h)
	{
		addr = (int *)(player->frames[frame].image_pixel_ptr + (i
					* player->frames[frame].line_len));
		addr2 = (int *)(data->img.image_pixel_ptr + ((i + start_y)
					* data->img.line_len));
		j = 0;
		while (j < player->player_w)
		{
			if ((unsigned int)addr[j] != 0xFF000000 && (j + start_x) >= 0 && (j
					+ start_x) < WINDOW_WIDTH && (i + start_y) >= 0 && (i
					+ start_y) < WINDOW_HEIGHT)
				addr2[j + start_x] = addr[j];
			j++;
		}
		i++;
	}
}
