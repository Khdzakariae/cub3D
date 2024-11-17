#include "main.h"

bool		load_cub3d_file(char *file, t_cub3d *cub3d);
char   		 **get_lines(int fd, char **cube_file, int *line_count);
static bool	file_is_valid(char **cube_file, t_cub3d **cub3d);

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
