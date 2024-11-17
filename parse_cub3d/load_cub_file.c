#include "main.h"

char   		 **get_lines(int fd, char **cube_file, int *line_count);
static bool	file_is_valid(char **cube_file, t_cub3d **cub3d);
void		load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void		fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
int			get_map_height(char **cube_file, size_t map_starting_i);
void		copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i);
void		get_map_line_len(char *line, size_t *line_map_len);
bool 		valid_map_char(char c);

bool	load_cub3d_file(char *file, t_cub3d *cub3d)
{
	int fd;
	char **cube_file;
	int	line_count;

	line_count = 0;
	fd = open(file, O_RDONLY);
	if (!fd)
		return (false);
	// parse cub name file
	if (check_cub_extension(file) == false)
		return (printf("Error\nInvalid file extension\n"), close(fd), false);
	// a cube_file version with no empty lines and trimmed spaces.
	cube_file = get_lines(fd, cube_file, &line_count);
	if (!line_count || !cube_file)
		return (printf("Error\nEmpty file\n"), close(fd), false);
	init_cub3d_data(&cub3d);
	if (file_is_valid(cube_file, &cub3d) == false)
		return (printf("Error\nInvalid cube file\n"), free(cube_file), close(fd), false);  // TODO free 2d ARRAY.
	load_cub3d_map(cube_file, &cub3d);
	// parse cub file
	return (true);
}

char    **get_lines(int fd, char **cube_file, int *line_count)
{
	int len;
	char *line;

	line = NULL;
	while ((len = get_next_line(fd, &line)) > 0)
	{
		// remove newline character
		line[len - 1] = '\0';
		// skip empty lines and lines with only spaces
		if (*line == '\0' || space_checker(line) == true)
		{
			free(line);
			continue;
		}
        *line_count += 1;
        cube_file = ft_realloc(cube_file, *line_count);
        cube_file[*line_count - 1] = ft_strtrim(line, " ");
		free(line);
	}
    if (len == -1)
    {
        free(cube_file);
        close(fd);
        printf("Error\nget_next_line failed\n");
        exit(EXIT_FAILURE);
    }
    if (*line_count == 0)
        return (free(cube_file), close(fd), NULL);
    cube_file[*line_count] = NULL;
    return (cube_file);
}

static bool file_is_valid(char **cube_file, t_cub3d **cub3d)
{
	size_t i;

	i = 0;
	// TODO: handle if the textures are not set or set twice.
	while(cube_file[i])
	{
		if (!ft_strncmp("NO ", cube_file[i], 3) && (*cub3d)->textures.no.is_set == false)
			(*cub3d)->textures.no.is_set = true;
		else if (!ft_strncmp("SO ", cube_file[i], 3) && (*cub3d)->textures.so.is_set == false)
			(*cub3d)->textures.so.is_set = true;
		else if (!ft_strncmp("WE ", cube_file[i], 3) && (*cub3d)->textures.we.is_set == false)
			(*cub3d)->textures.we.is_set = true;
		else if (!ft_strncmp("EA ", cube_file[i], 3) && (*cub3d)->textures.ea.is_set == false)
			(*cub3d)->textures.ea.is_set = true;
		else if (!ft_strncmp("F ", cube_file[i], 2) && (*cub3d)->colors.floor.is_set == false)
			(*cub3d)->colors.floor.is_set = true;
		else if (!ft_strncmp("C ", cube_file[i], 2) && (*cub3d)->colors.ceiling.is_set == false)
			(*cub3d)->colors.ceiling.is_set = true;
		else if (cube_file[i][0] != '1')
		{
			printf("Error\nInvalid line\n");
			// TODO free what needs to be freed.
			return (false);
		}
		i++;
	}
	if ((*cub3d)->textures.no.is_set == false 
		|| (*cub3d)->textures.so.is_set == false 
		|| (*cub3d)->textures.we.is_set == false 
		|| (*cub3d)->textures.ea.is_set == false 
		|| (*cub3d)->colors.floor.is_set == false 
		|| (*cub3d)->colors.ceiling.is_set == false)
	{
		printf("Error\nMissing texture or color\n");
		// TODO free what needs to be freed.
		return (false);
	}
	return (true);
}

void	load_cub3d_map(char **cube_file, t_cub3d **cub3d_data)
{
	bool map;
	size_t i;
	size_t j;

	map = false;
	i = 0;
	j = 0;
	while (cube_file[i] != NULL)
	{
		if (cube_file[i][0] == '1' && map == false)
		{
			map = true;
			// fill the map on cub3d_data, and dirreference the index.
			fill_map(cube_file, cub3d_data, &i);
		}
		else if (cube_file[i][0] == '1' && map == true)
		{
			printf("Error\nInvalid map\n");
			// have to free what needs to be freed. TODO.
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i)
{
	size_t map_starting_i;
	size_t map_height;

	map_starting_i = *i;
	map_height = get_map_height(cube_file, map_starting_i);
	*i += map_height;
	(*cub3d_data)->map.height = map_height;
	(*cub3d_data)->map.map = (char **)malloc(sizeof(char *) * map_height + 1);
	if (!(*cub3d_data)->map.map)
	{
		printf("Error\nFailed to allocate memory\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	// fill the map
		// skip spaces, and check for valid characters when filling the map.
		// check if the map is surrounded by walls.
	copy_map_content(cube_file, cub3d_data, map_starting_i);
	// print map
	for (size_t i = 0; i < map_height; i++)
		printf("%s\n", (*cub3d_data)->map.map[i]);
}

int	get_map_height(char **cube_file, size_t map_starting_i)
{
	size_t i;

	i = map_starting_i;
	while (cube_file[i] != NULL && cube_file[i][0] == '1')
		i++;
	return (i - map_starting_i);
}

void	copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i)
{
	size_t map_j;
	size_t j;
	size_t line_map_len;
	size_t map_i;
	size_t map_height_scaled;

	line_map_len = 0;
	map_i = 0;
	map_height_scaled = (*cub3d_data)->map.height + map_starting_i;
	while (map_starting_i < map_height_scaled)
	{
		map_j = 0;
		j = 0;
		get_map_line_len(cube_file[map_starting_i], &line_map_len);
		(*cub3d_data)->map.map[map_i] = (char *)malloc(sizeof(char) * line_map_len + 1);
		while (cube_file[map_starting_i][j] != '\0')
		{
			if (valid_map_char(cube_file[map_starting_i][j]) == false)
			{
				printf("Error\nInvalid character in map\n");
				// TODO free what needs to be freed.
				exit(EXIT_FAILURE);
			}
			if (ft_isspace(cube_file[map_starting_i][j]) == false)
			{
				(*cub3d_data)->map.map[map_i][map_j] = cube_file[map_starting_i][j];
				map_j++;
			}
			j++;
		}
		(*cub3d_data)->map.map[map_i][map_j] = '\0';
		map_i++;
		map_starting_i++;
	}
	(*cub3d_data)->map.map[map_i] = NULL;
	// print map
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

bool	valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || c == 'W' || c == 'E' || c == ' ');
}
