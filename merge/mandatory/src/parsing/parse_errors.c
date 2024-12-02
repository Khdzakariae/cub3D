/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:10:38 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/26 13:29:03 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parsing.h>

void	err_exit(char *err_msg, t_cub3d **cub3d_data);
void	print_exit(const char *err_msg);
void	free_incomplete_map(t_cub3d **cub3d_data, size_t map_i, char *err_msg);
void	free_arrays_exit(char **arr1, char **arr2,
			char *err_msg, t_cub3d **cub3d);

void	err_exit(char *err_msg, t_cub3d **cub3d_data)
{
	ssize_t	err;

	if (err_msg)
		err = write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	(void)err;
	if (cub3d_data == NULL)
		exit(EXIT_FAILURE);
	if ((*cub3d_data)->fd > 2)
		close((*cub3d_data)->fd);
	if ((*cub3d_data)->cube_file)
		free_2d_array((*cub3d_data)->cube_file);
	if ((*cub3d_data)->map.map)
		free_2d_array((*cub3d_data)->map.map);
	if ((*cub3d_data)->textures.ea.path)
		free((*cub3d_data)->textures.ea.path);
	if ((*cub3d_data)->textures.we.path)
		free((*cub3d_data)->textures.we.path);
	if ((*cub3d_data)->textures.so.path)
		free((*cub3d_data)->textures.so.path);
	if ((*cub3d_data)->textures.no.path)
		free((*cub3d_data)->textures.no.path);
	exit(EXIT_FAILURE);
}

void	print_exit(const char *err_msg)
{
	ssize_t	err;

	err = write(2, err_msg, ft_strlen(err_msg));
	(void)err;
	exit(EXIT_FAILURE);
}

void	free_incomplete_map(t_cub3d **cub3d_data, size_t map_i, char *err_msg)
{
	size_t	i;

	i = 0;
	while (i <= map_i)
	{
		free((*cub3d_data)->map.map[i]);
		i++;
	}
	free((*cub3d_data)->map.map);
	(*cub3d_data)->map.map = NULL;
	err_exit(err_msg, cub3d_data);
}

void	free_arrays_exit(char **arr1, char **arr2,
			char *err_msg, t_cub3d **cub3d)
{
	if (arr1)
		free_2d_array(arr1);
	if (arr2)
		free_2d_array(arr2);
	err_exit(err_msg, cub3d);
}
