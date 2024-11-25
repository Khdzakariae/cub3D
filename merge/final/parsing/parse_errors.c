#include "../Includes/parsing.h"

void	err_exit(char *err_msg, t_cub3d **cub3d_data)
{
	ssize_t err;

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
	ssize_t err;

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
