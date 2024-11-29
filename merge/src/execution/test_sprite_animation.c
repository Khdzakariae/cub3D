/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_sprite_animation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 18:42:55 by zel-khad          #+#    #+#             */
/*   Updated: 2024/11/29 12:51:15 by zel-khad         ###   ########.fr       */
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
	printf("get path of %d\n", current_image);
	assets = ft_calloc(20, sizeof(char));
	number = ft_itoa(current_image);
	strcat(assets, "knife_tmp/");
	strcat(assets, number);
	strcat(assets, ".xpm");
	free(number);
	return (assets);
}

int	update_frame(t_data *data)
{
	static int	n_img = 0;
	char		*assets;

	if (n_img == PLAYER_FRAMES)
		n_img = 0;
	assets = get_assets(n_img);
	data->game.player.frames[n_img++].path = assets;
	return (1);
}
