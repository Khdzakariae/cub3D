/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:11:00 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 13:29:17 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing_bonus.h>

void	get_map_dimensions(char **cube_file, size_t map_starting_i,
			size_t *map_height, size_t *map_width);
void	get_map_line_len(char *line, size_t *line_map_len);
t_bool	valid_map_char(char c);
void	free_2d_array(char **array);
t_bool	all_cub3d_data_set(t_cub3d **cub3d);

void	get_map_dimensions(char **cube_file, size_t map_starting_i,
			size_t *map_height, size_t *map_width)
{
	size_t	i;
	size_t	j;
	size_t	max_map_width;

	i = map_starting_i;
	max_map_width = 0;
	while (cube_file[i] != NULL)
	{
		if (check_for_wall(cube_file[i]) == false)
		{
			*map_height = i - map_starting_i;
			*map_width = max_map_width;
			return ;
		}
		j = 0;
		while (cube_file[i][j])
			j++;
		if (j > max_map_width)
			max_map_width = j;
		i++;
	}
	*map_height = i - map_starting_i;
	*map_width = max_map_width;
}

void	get_map_line_len(char *line, size_t *line_map_len)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (ft_isspace(line[i]) == false)
			*line_map_len += 1;
		i++;
	}
}

void	free_2d_array(char **array)
{
	size_t	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

t_bool	all_cub3d_data_set(t_cub3d **cub3d)
{
	return ((*cub3d)->textures.no.is_set == true
		&& (*cub3d)->textures.so.is_set == true
		&& (*cub3d)->textures.we.is_set == true
		&& (*cub3d)->textures.ea.is_set == true
		&& (*cub3d)->colors.floor.is_set == true
		&& (*cub3d)->colors.ceiling.is_set == true);
}

t_bool	check_for_wall(char *line)
{
	while (*line && line)
	{
		if (*line == '1')
			return (true);
		else if (ft_isspace(*line) == true)
			line++;
		else
			return (false);
	}
	return (false);
}
