#include "../Includes/parsing.h"

void		load_cub3d_file(char *file, t_cub3d *cub3d);
char   		 **get_lines(int fd, char **cube_file, int *line_count, t_cub3d **cub3d_data);
static void	file_is_valid(char **cube_file, t_cub3d **cub3d);
static void	fetch_textures(char **cube_file, t_cub3d **cub3d);
void		get_texture_path(char *line, char **path, char **cube_file, t_cub3d **cub3d_data);
void		fetch_colors(char **cube_file, t_cub3d **cub3d);
void		get_color(char *line, int *color, t_cub3d **cub3d, char **cube_file);
void		retrieve_color(char **split_rgb, int *color, t_cub3d **cub3d, char **cube_file);

void	load_cub3d_file(char *file, t_cub3d *cub3d)
{
	int fd;
	char **cube_file;
	int	line_count;

	line_count = 0;
	cube_file = NULL;
	fd = open(file, O_RDONLY);
	if (!fd)
		err_exit("Error\nfile can not open\n", NO_FD, NULL, NULL, NULL);
	// parse cub name file
	if (check_cub_extension(file) == false)
		err_exit("Error\nInvalid file extension\n", fd, NULL, NULL, NULL);
	// a cube_file version with no empty lines and trimmed spaces.
	cube_file = get_lines(fd, cube_file, &line_count, &cub3d);
	init_cub3d_data(&cub3d, fd);
	file_is_valid(cube_file, &cub3d);
	load_cub3d_map(cube_file, &cub3d);
	// load textures and colors
	fetch_textures(cube_file, &cub3d);
	// check paths
/* 	 printf("NO: %s\n", cub3d->textures.no.path);
	printf("SO: %s\n", cub3d->textures.so.path);
	printf("WE: %s\n", cub3d->textures.we.path);
	printf("EA: %s\n", cub3d->textures.ea.path); */
	fetch_colors(cube_file, &cub3d);
	// check colors
/* 	printf("F: %x\n", cub3d->colors.floor.color);
	printf("C: %x\n", cub3d->colors.ceiling.color);
	printf("Map width: %d\n", cub3d->map.width);
    printf("Map height: %d\n", cub3d->map.height); */
    // print map
/*     for (size_t i = 0; i < cub3d->map.height; i++)
        printf("%s\n", cub3d->map.map[i]);  */
	// update players position according to the resolution screen.
	cub3d->player.x = cub3d->map.player_x * TILE_SIZE;
	cub3d->player.y = cub3d->map.player_y * TILE_SIZE;
	free_2d_array(cube_file);
}

char    **get_lines(int fd, char **cube_file, int *line_count, t_cub3d **cub3d)
{
	int len;
	char *line;
	char	*lines;
	t_bool	map;

	line = NULL;
	lines = NULL;
	map = false;
	while ((len = get_next_line(fd, &line)) > 0)
	{
		if (*line == '1')
			map = true;
		// skip empty lines and lines with only spaces
		else if ((*line == '\0' || space_checker(line) == true) && map == false)
		{
			free(line);
			continue;
		}
		if (*line == '\n')
		{
			line = ft_strjoin(line, "\n");
			*line = ' ';
		}
		lines = ft_strjoin(lines, line);
		free(line);
		*line_count += 1;
	}
	if (*line_count == 0)
        err_exit("Error\nEmpty file\n", fd, cube_file, NULL, NULL);
	if (len == -1)
		err_exit("Error\nget_next_line failed\n", fd, cube_file, NULL, cub3d);
	cube_file = ft_split(lines, '\n');
	for (int i = 0; cube_file[i] ;i++)
		cube_file[i] = ft_strtrim(cube_file[i], " ");
    return (free(lines), cube_file);
}

static void file_is_valid(char **cube_file, t_cub3d **cub3d)
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
		else if (cube_file[i][0] != '1' && cube_file[i][0] != '\0')
			err_exit("Error\nInvalid line, texture or color\n", (*cub3d)->fd, cube_file, NULL, cub3d);
		else if (cube_file[i][0] == '1')
		{
			// check if the map is the last element
			if ((*cub3d)->textures.no.is_set == false 
				|| (*cub3d)->textures.so.is_set == false 
				|| (*cub3d)->textures.we.is_set == false 
				|| (*cub3d)->textures.ea.is_set == false 
				|| (*cub3d)->colors.floor.is_set == false 
				|| (*cub3d)->colors.ceiling.is_set == false)
				err_exit("Error\nMap must be last element or Texture's missing\n", (*cub3d)->fd, cube_file, NULL, cub3d);
		}			
		i++;
	}
	if ((*cub3d)->textures.no.is_set == false 
		|| (*cub3d)->textures.so.is_set == false 
		|| (*cub3d)->textures.we.is_set == false 
		|| (*cub3d)->textures.ea.is_set == false 
		|| (*cub3d)->colors.floor.is_set == false 
		|| (*cub3d)->colors.ceiling.is_set == false)
		err_exit("Error\nMissing texture or color\n", (*cub3d)->fd, cube_file, NULL, cub3d);
}

void	fetch_textures(char **cube_file, t_cub3d **cub3d_data)
{
	size_t i;

	i = 0;
	while (cube_file[i])
	{
		if (!ft_strncmp("NO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.no.path, cube_file, cub3d_data);
		else if (!ft_strncmp("SO ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.so.path, cube_file, cub3d_data);
		else if (!ft_strncmp("WE ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.we.path, cube_file, cub3d_data);
		else if (!ft_strncmp("EA ", cube_file[i], 3))
			get_texture_path(cube_file[i], &(*cub3d_data)->textures.ea.path, cube_file, cub3d_data);
		i++;
	}
}

void	get_texture_path(char *line, char **path, char **cube_file, t_cub3d **cub3d_data)
{
	char **split_textures;

	split_textures = ft_split(line, ' ');
	if (!split_textures)
	{
		free_2d_array(split_textures);
		err_exit("Error\nFailed to split textures\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	}
	else if (!split_textures[1])
	{
		free_2d_array(split_textures);
		err_exit("Error\nInvalid texture path\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	}
	else if (split_textures[2])
	{
		free_2d_array(split_textures);
		err_exit("Error\nInvalid texture path\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
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
			get_color(cube_file[i], &(*cub3d_data)->colors.floor.color, cub3d_data, cube_file);
		else if (!ft_strncmp("C ", cube_file[i], 2))
			get_color(cube_file[i], &(*cub3d_data)->colors.ceiling.color, cub3d_data, cube_file);
		i++;
	}
}

void	get_color(char *line, int *color, t_cub3d **cub3d_data, char **cube_file)
{
	char **split_colors;
	char **split_rgb;

	split_colors = ft_split(line, ' ');
	if (!split_colors)
		err_exit("Error\nFailed to split colors\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	else if (!split_colors[1])
		err_exit("Error\nInvalid color\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	else if (split_colors[2])
		err_exit("Error\nInvalid color\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	split_rgb = ft_split(split_colors[1], ',');
	if (!split_rgb)
		err_exit("Error\nFailed to split colors\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	else if (!split_rgb[0] || !split_rgb[1] || !split_rgb[2])
		err_exit("Error\nInvalid color\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	retrieve_color(split_rgb, color, cub3d_data, cube_file);
	free_2d_array(split_colors);
	free_2d_array(split_rgb);
}

void 	retrieve_color(char **split_rgb, int *color, t_cub3d **cub3d_data, char **cube_file)
{
	int r;
	int g;
	int b;

	r = ft_atoi(split_rgb[0]);
	g = ft_atoi(split_rgb[1]);
	b = ft_atoi(split_rgb[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		err_exit("Error\nInvalid color\n", (*cub3d_data)->fd, cube_file, (*cub3d_data)->map.map, cub3d_data);
	*color = (r << 16) | (g << 8) | b;
}
