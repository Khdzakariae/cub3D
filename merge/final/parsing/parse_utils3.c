#include "../Includes/parsing.h"

void	get_map_dimensions(char **cube_file, size_t map_starting_i, size_t *map_height, size_t *map_width);
void	get_map_line_len(char *line, size_t *line_map_len);
t_bool 	valid_map_char(char c);
void 	free_2d_array(char **array);
void	fetch_min_map_width(char **map, size_t *min_width);

void	get_map_dimensions(char **cube_file, size_t map_starting_i, size_t *map_height, size_t *map_width)
{
	size_t i;
	size_t j;
	size_t	max_map_width;

	i = map_starting_i;
	max_map_width = 0;
	while (cube_file[i] != NULL && cube_file[i][0] == '1' && cube_file[i][0])
	{
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

t_bool	all_cub3d_data_set(t_cub3d **cub3d)
{
	return ((*cub3d)->textures.no.is_set == true
			&& (*cub3d)->textures.so.is_set == true 
			&& (*cub3d)->textures.we.is_set == true 
			&& (*cub3d)->textures.ea.is_set == true 
			&& (*cub3d)->colors.floor.is_set == true 
			&& (*cub3d)->colors.ceiling.is_set == true);
}
