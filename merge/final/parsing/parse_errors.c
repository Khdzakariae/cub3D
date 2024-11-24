#include "../Includes/parsing.h"

void	err_exit(const char *err_msg, int close_fd, char **cube_file, char **map,
			t_cub3d **cub3d_data)
{
	ssize_t err;

	if (err_msg)
		err = write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	if (close_fd > 2)
		close(close_fd);
	if (cube_file)
		free_2d_array(cube_file);
	if (map)
		free_2d_array(map);
	if (cub3d_data)
	{
		if ((*cub3d_data)->textures.ea.path)
			free((*cub3d_data)->textures.ea.path);
		if ((*cub3d_data)->textures.we.path)
			free((*cub3d_data)->textures.we.path);
		if ((*cub3d_data)->textures.so.path)
			free((*cub3d_data)->textures.so.path);
		if ((*cub3d_data)->textures.no.path)
			free((*cub3d_data)->textures.no.path);
	}
	(void)err;
	exit(EXIT_FAILURE);
}

void	print_exit(const char *err_msg)
{
	ssize_t err;

	err = write(2, err_msg, ft_strlen(err_msg));
	(void)err;
	exit(EXIT_FAILURE);
}
