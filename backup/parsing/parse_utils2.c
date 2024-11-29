/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:53 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 13:29:11 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

void	get_lines(int *line_count, t_cub3d **cub3d_data);
void	fetch_lines(t_cub3d **cub3d, char **lines,
			t_bool *map, int *line_count);
void	check_file_lines(char **cube_file, t_cub3d **cub3d, size_t i);
void	get_texture_path(char *line, char **path, t_cub3d **cub3d_data);
t_bool	check_if_all_walls(char *line);

void	get_lines(int *line_count, t_cub3d **cub3d)
{
	char	*lines;
	t_bool	map;
	size_t	i;

	lines = NULL;
	map = false;
	i = 0;
	fetch_lines(cub3d, &lines, &map, line_count);
	if (*line_count == 0)
		err_exit("Error\nEmpty file\n", cub3d);
	(*cub3d)->cube_file = ft_split(lines, '\n');
	while ((*cub3d)->cube_file[i])
	{
		(*cub3d)->cube_file[i] = ft_strtrim((*cub3d)->cube_file[i], " ");
		i++;
	}
	free(lines);
}

void	fetch_lines(t_cub3d **cub3d, char **lines,
			t_bool *map, int *line_count)
{
	int		len;
	char	*line;

	line = NULL;
	while (get_next_line_len((*cub3d)->fd, &line, &len) > 0)
	{
		if (*line == '1')
			*map = true;
		else if ((*line == '\0' || space_checker(line) == true)
			&& *map == false)
		{
			free(line);
			continue ;
		}
		if (*line == '\n')
		{
			line = ft_strjoin(line, "\n");
			*line = ' ';
		}
		*lines = ft_strjoin(*lines, line);
		free(line);
		*line_count += 1;
	}
	if (len == -1)
		err_exit("Error\nget_next_line failed\n", cub3d);
}

void	check_file_lines(char **cube_file, t_cub3d **cub3d, size_t i)
{
	if (!ft_strncmp("NO ", cube_file[i], 3)
		&& (*cub3d)->textures.no.is_set == false)
		(*cub3d)->textures.no.is_set = true;
	else if (!ft_strncmp("SO ", cube_file[i], 3)
		&& (*cub3d)->textures.so.is_set == false)
		(*cub3d)->textures.so.is_set = true;
	else if (!ft_strncmp("WE ", cube_file[i], 3)
		&& (*cub3d)->textures.we.is_set == false)
		(*cub3d)->textures.we.is_set = true;
	else if (!ft_strncmp("EA ", cube_file[i], 3)
		&& (*cub3d)->textures.ea.is_set == false)
		(*cub3d)->textures.ea.is_set = true;
	else if (!ft_strncmp("F ", cube_file[i], 2)
		&& (*cub3d)->colors.floor.is_set == false)
		(*cub3d)->colors.floor.is_set = true;
	else if (!ft_strncmp("C ", cube_file[i], 2)
		&& (*cub3d)->colors.ceiling.is_set == false)
		(*cub3d)->colors.ceiling.is_set = true;
	else if (cube_file[i][0] != '1' && cube_file[i][0] != '\0')
		err_exit("Error\nInvalid line, texture or color\n", cub3d);
	else if (cube_file[i][0] == '1')
	{
		if (all_cub3d_data_set(cub3d) == false)
			err_exit("Error\nTexture's missing or map not last\n", cub3d);
	}
}

void	get_texture_path(char *line, char **path, t_cub3d **cub3d_data)
{
	char	**split_textures;

	split_textures = ft_split(line, ' ');
	if (!split_textures)
	{
		free_2d_array(split_textures);
		err_exit("Error\nFailed to split textures\n", cub3d_data);
	}
	else if (!split_textures[1])
	{
		free_2d_array(split_textures);
		err_exit("Error\nInvalid texture path\n", cub3d_data);
	}
	else if (split_textures[2])
	{
		free_2d_array(split_textures);
		err_exit("Error\nInvalid texture path\n", cub3d_data);
	}
	*path = ft_strdup(split_textures[1]);
	if (*path == NULL)
	{
		free_2d_array(split_textures);
		err_exit("Error\ntexture's path alloc failed\n", cub3d_data);
	}
	free_2d_array(split_textures);
}

t_bool	check_if_all_walls(char *line)
{
	while (*line)
	{
		if (*line != '1')
			return (false);
		line++;
	}
	return (true);
}
