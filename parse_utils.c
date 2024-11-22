#include "main.h"

t_bool		check_cub_extension(char *file);
void		init_cub3d_data(t_cub3d **cub3d, int fd);
int			get_map_height(char **cube_file, size_t map_starting_i);
void		get_map_line_len(char *line, size_t *line_map_len);
t_bool 		valid_map_char(char c);
t_bool		check_if_all_walls(char *line);
void 		free_2d_array(char **array);

t_bool	check_cub_extension(char *file)
{
	while (file && *file)
	{
		if (*file == '.' && *(file + 1) == 'c' && *(file + 2) == 'u' && *(file + 3) == 'b' && *(file + 4) == '\0')
			return (true);
		file++;
	}
	return (false);
}
void	init_cub3d_data(t_cub3d **cub3d, int fd)
{
	(*cub3d)->textures.no.path = NULL;
	(*cub3d)->textures.no.is_set = false;
	(*cub3d)->textures.so.path = NULL;
	(*cub3d)->textures.so.is_set = false;
	(*cub3d)->textures.we.path = NULL;
	(*cub3d)->textures.we.is_set = false;
	(*cub3d)->textures.ea.path = NULL;
	(*cub3d)->textures.ea.is_set = false;
	(*cub3d)->colors.floor.color = -1;
	(*cub3d)->colors.floor.is_set = false;
	(*cub3d)->colors.ceiling.color = -1;
	(*cub3d)->colors.ceiling.is_set = false;
	(*cub3d)->map.map = NULL;
	(*cub3d)->map.width = 0;
	(*cub3d)->map.height = 0;
	(*cub3d)->map.title_size = TILE_SIZE;
	(*cub3d)->map.player_x = -1;
	(*cub3d)->map.player_y = -1;
	(*cub3d)->fd = fd;
}

int	get_map_height(char **cube_file, size_t map_starting_i)
{
	size_t i;

	i = map_starting_i;
	while (cube_file[i] != NULL && cube_file[i][0] == '1')
		i++;
	return (i - map_starting_i);
}

void	get_map_line_len(char *line, size_t *line_map_len)
{
	size_t i;

	i = 0;
	while (line[i] != '\0')
	{
		if (ft_isspace(line[i]) == false)
			*line_map_len += 1;
		i++;
	}
}

t_bool	valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'W' || c == 'E' || c == ' ');
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

void	free_2d_array(char **array)
{
	size_t i;

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
