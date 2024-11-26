/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_cub3d_map_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achahid- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:19 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 08:10:20 by achahid-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/parsing.h"

void		map_is_valid(t_cub3d **cub3d_data);
static void	check_player_escapes(t_cub3d **cub3d_data);
static void	content_is_valid(t_cub3d **cub3d_data);
static void	check_content(t_cub3d **cub3d_data, t_bool *player,
				t_bool *spaces, size_t i);

void	map_is_valid(t_cub3d **cub3d_data)
{
	size_t	i;
	int		len_line;

	i = 0;
	check_player_escapes(cub3d_data);
	while ((*cub3d_data)->map.map[i] != NULL)
	{
		len_line = ft_strlen((*cub3d_data)->map.map[i]);
		if (i == 0 || i == (size_t)(*cub3d_data)->map.height - 1)
		{
			if (check_if_all_walls((*cub3d_data)->map.map[i]) == false)
				err_exit("Error\nMap is not surrounded by walls\n", cub3d_data);
		}
		else if ((*cub3d_data)->map.map[i][0] != '1'
				|| (*cub3d_data)->map.map[i][len_line - 1] != '1')
			err_exit("Error\nMap is not surrounded by walls\n", cub3d_data);
		i++;
	}
	content_is_valid(cub3d_data);
}

static void	check_player_escapes(t_cub3d **cub3d_data)
{
	size_t	min_width;
	size_t	i;
	size_t	j;

	min_width = MAX_INT;
	i = 0;
	fetch_min_map_width((*cub3d_data)->map.map, &min_width);
	while ((*cub3d_data)->map.map[i])
	{
		j = 0;
		while ((*cub3d_data)->map.map[i][j])
		{
			if (j >= min_width && (*cub3d_data)->map.map[i][j] != '1'
				&& i != 0 && (int)i < (*cub3d_data)->map.height - 1)
			{
				if ((*cub3d_data)->map.map[i - 1][j] == '\0'
					|| (*cub3d_data)->map.map[i + 1][j] == '\0')
					err_exit("Error\nPlayer can escape, Rja3 a W9\n",
						cub3d_data);
			}
			j++;
		}
		i++;
	}
}

static void	content_is_valid(t_cub3d **cub3d_data)
{
	size_t	i;
	t_bool	player;
	t_bool	spaces;

	i = 0;
	player = false;
	spaces = false;
	while ((*cub3d_data)->map.map[i] != NULL)
	{
		check_content(cub3d_data, &player, &spaces, i);
		i++;
	}
	if (player == false || spaces == false)
		err_exit("Error\nNo player position or no spaces\n", cub3d_data);
}

static void	check_content(t_cub3d **cub3d_data, t_bool *player,
				t_bool *spaces, size_t i)
{
	size_t	j;

	j = 0;
	while ((*cub3d_data)->map.map[i][j] != '\0')
	{
		if ((*cub3d_data)->map.map[i][j] == 'N'
			|| (*cub3d_data)->map.map[i][j] == 'S'
			|| (*cub3d_data)->map.map[i][j] == 'W'
			|| (*cub3d_data)->map.map[i][j] == 'E')
		{
			if (*player == true)
				err_exit("Error\nMultiple player position\n", cub3d_data);
			*player = true;
			(*cub3d_data)->map.player_x = j;
			(*cub3d_data)->map.player_y = i;
			(*cub3d_data)->player.player_direction
				= ((*cub3d_data)->map.map[i][j]);
		}
		else if ((*cub3d_data)->map.map[i][j] == '0')
			*spaces = true;
		j++;
	}
}
