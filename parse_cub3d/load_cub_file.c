#include "main.h"

bool		load_cub3d_file(char *file, t_cub3d *cub3d);
char   		 **get_lines(int fd, char **cube_file, int *line_count);
static bool	file_is_valid(char **cube_file, t_cub3d **cub3d);
static void	fetch_textures(char **cube_file, t_cub3d **cub3d);
void		get_texture_path(char *line, char **path);
void		fetch_colors(char **cube_file, t_cub3d **cub3d);
void		get_color(char *line, int *color);
void		retrieve_color(char **split_rgb, int *color);

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
	// load textures and colors
	fetch_textures(cube_file, &cub3d);
	// check paths
	printf("NO: %s\n", cub3d->textures.no.path);
	printf("SO: %s\n", cub3d->textures.so.path);
	printf("WE: %s\n", cub3d->textures.we.path);
	printf("EA: %s\n", cub3d->textures.ea.path);
	fetch_colors(cube_file, &cub3d);
	// check colors
	printf("F: %x\n", cub3d->colors.floor.color);
	printf("C: %x\n", cub3d->colors.ceiling.color);
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
			printf("Error\nInvalid line, texture or color\n");
			// TODO free what needs to be freed.
			return (false);
		}
		else if (cube_file[i][0] == '1')
		{
			// check if the map is the last element
			if ((*cub3d)->textures.no.is_set == false 
				|| (*cub3d)->textures.so.is_set == false 
				|| (*cub3d)->textures.we.is_set == false 
				|| (*cub3d)->textures.ea.is_set == false 
				|| (*cub3d)->colors.floor.is_set == false 
				|| (*cub3d)->colors.ceiling.is_set == false)
			{
				printf("Error\nMap must be last element or Texture missing\n");
				// TODO free what needs to be freed.
				exit(EXIT_FAILURE);
			}
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

void	fetch_textures(char **cube_file, t_cub3d **cub3d_data)
{
	size_t i;

	i = 0;
	while (cube_file[i])
	{
		if (!ft_strncmp("NO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.no.path);
		else if (!ft_strncmp("SO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.so.path);
		else if (!ft_strncmp("WE ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.we.path);
		else if (!ft_strncmp("EA ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.ea.path);
		i++;
	}
}

void	get_texture_path(char *line, char **path)
{
	char **split_textures;

	split_textures = ft_split(line, ' ');
	if (!split_textures)
	{
		printf("Error\nFailed to split textures\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	else if (!split_textures[1])
	{
		printf("Error\nInvalid texture path\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	else if (split_textures[2])
	{
		printf("Error\nInvalid texture path\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	*path = ft_strdup(split_textures[1]);
	free_2d_array(split_textures);
}

void	fetch_colors(char **cube_file, t_cub3d **cub3d_data)
{
	size_t i;

	i = 0;
	while (cube_file[i] != NULL)
	{
		if (!ft_strncmp("F ", cube_file[i], 2))
			get_color(cube_file[i], &(*cub3d_data)->colors.floor.color);
		else if (!ft_strncmp("C ", cube_file[i], 2))
			get_color(cube_file[i], &(*cub3d_data)->colors.ceiling.color);
		i++;
	}
}

void	get_color(char *line, int *color)
{
	char **split_colors;
	char **split_rgb;

	split_colors = ft_split(line, ' ');
	if (!split_colors)
	{
		printf("Error\nFailed to split colors\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	else if (!split_colors[1])
	{
		printf("Error\nInvalid color\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	else if (split_colors[2])
	{
		printf("Error\nInvalid color\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	split_rgb = ft_split(split_colors[1], ',');
	if (!split_rgb)
	{
		printf("Error\nFailed to split colors\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	else if (!split_rgb[0] || !split_rgb[1] || !split_rgb[2])
	{
		printf("Error\nInvalid color\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	retrieve_color(split_rgb, color);
	free_2d_array(split_colors);
	free_2d_array(split_rgb);
}

void 	retrieve_color(char **split_rgb, int *color)
{
	int r;
	int g;
	int b;

	r = ft_atoi(split_rgb[0]);
	g = ft_atoi(split_rgb[1]);
	b = ft_atoi(split_rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		printf("Error\nInvalid color\n");
		// TODO free what needs to be freed.
		exit(EXIT_FAILURE);
	}
	*color = (r << 16) | (g << 8) | b;
}
