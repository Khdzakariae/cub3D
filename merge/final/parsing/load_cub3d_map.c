#include "../Includes/parsing.h"

void	load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void	fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
void	copy_map_content(char **cube_file, t_cub3d **cub3d_data,
			size_t map_starting_i);
void	move_content_to_map(char **cube_file, t_cub3d **cub3d_data,
			size_t map_starting_i, size_t map_i);
void	map_is_valid(t_cub3d **cub3d_data);
void	check_player_escapes(t_cub3d **cub3d_data);
void	check_player_escapes(t_cub3d **cub3d_data);
void	content_is_valid(t_cub3d **cub3d_data);
void	check_content(t_cub3d **cub3d_data, t_bool *player,
			t_bool *spaces, size_t i);

void	load_cub3d_map(char **cube_file, t_cub3d **cub3d_data)
{
	t_bool map;
	size_t i;
	map = false;
	i = 0;
	while (cube_file[i] != NULL)
	{
		if (cube_file[i][0] == '1' && map == false)
		{
			map = true;
			fill_map(cube_file, cub3d_data, &i);
			map_is_valid(cub3d_data);
			if (cube_file[i] == NULL)
				break ;
		}
		else if (cube_file[i][0] == '1' && map == true)
			err_exit("Error\nOnly one map is valid\n", cub3d_data);
		i++;
	}
	if ((*cub3d_data)->map.width == 0 || (*cub3d_data)->map.height == 0)
		err_exit("Error\nNo map found\n", cub3d_data);
}

void	fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i)
{
	size_t map_starting_i;
	size_t map_height;
	size_t  map_width;

	map_starting_i = *i;
	get_map_dimensions(cube_file, map_starting_i, &map_height, &map_width);
	*i += map_height;
	(*cub3d_data)->map.height = map_height;
	(*cub3d_data)->map.width = map_width;
	(*cub3d_data)->map.map = (char **)malloc(sizeof(char *) * (map_height + 1));
	if (!(*cub3d_data)->map.map)
		err_exit("Error\nFailed to allocte memory\n", cub3d_data);
	copy_map_content(cube_file, cub3d_data, map_starting_i);
}

void	copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i)
{
	size_t line_map_len;
	size_t map_i;
	size_t map_height_scaled;

	map_i = 0;
	map_height_scaled = (*cub3d_data)->map.height + map_starting_i;
	while (map_starting_i < map_height_scaled)
	{
		line_map_len = 0;
		(*cub3d_data)->map.map[map_i] = 
		(char *)malloc(sizeof(char) * (*cub3d_data)->map.width + 1);
		if ((*cub3d_data)->map.map[map_i] == NULL)
			free_incomplete_map(cub3d_data, map_i, "Error\nMap[i] allocation failed\n");
		move_content_to_map(cube_file, cub3d_data, map_starting_i, map_i);
		map_i++;
		map_starting_i++;
	}
	(*cub3d_data)->map.map[map_i] = NULL;
}

void    move_content_to_map(char **cube_file, t_cub3d **cub3d_data,
			size_t map_starting_i, size_t map_i)
{
	size_t  j;
	size_t  map_j;

	j = 0;
	map_j = 0;
	while (cube_file[map_starting_i][j] != '\0')
	{
		if (valid_map_char(cube_file[map_starting_i][j]) == false)
			free_incomplete_map(cub3d_data, map_i, "Error\nInvalid Character\n");
		if (ft_isspace(cube_file[map_starting_i][j]) == false)
		{
			(*cub3d_data)->map.map[map_i][map_j] = cube_file[map_starting_i][j];
			map_j++;
		}
		j++;
	}
	while (map_j < (*cub3d_data)->map.width)
	{
		(*cub3d_data)->map.map[map_i][map_j] = '\0';
		map_j++;
	}
	(*cub3d_data)->map.map[map_i][map_j] = '\0';
}

void    map_is_valid(t_cub3d **cub3d_data)
{
	size_t i;
	int     len_line;

	i = 0;
	check_player_escapes(cub3d_data);
	while ((*cub3d_data)->map.map[i] != NULL)
	{
		len_line = ft_strlen((*cub3d_data)->map.map[i]);
		if (i == 0 || i == (size_t)(*cub3d_data)->map.height - 1)
		{
			if (check_if_all_walls((*cub3d_data)->map.map[i]) == false)
				err_exit("Error\nMap is not surrounded by walls\n", cub3d_data);
		}
		else if ((*cub3d_data)->map.map[i][0] != '1' 
				|| (*cub3d_data)->map.map[i][len_line - 1] != '1')
			err_exit("Error\nMap is not surrounded by walls\n",  cub3d_data);
		i++;
	}
	content_is_valid(cub3d_data);
}

void    check_player_escapes(t_cub3d **cub3d_data)
{
	size_t  min_width;
	size_t  i;
	size_t  j;

	min_width = MAX_INT;
	i = 0;
	fetch_min_map_width((*cub3d_data)->map.map, &min_width);
	while ((*cub3d_data)->map.map[i])
	{
		j = 0;
		while ((*cub3d_data)->map.map[i][j])
		{
			if (j >= min_width && (*cub3d_data)->map.map[i][j] != '1'
				&& i != 0 && i < (*cub3d_data)->map.height - 1)
			{
				if ((*cub3d_data)->map.map[i - 1][j] == '\0' 
					|| (*cub3d_data)->map.map[i + 1][j] == '\0')
					err_exit("Error\nPlayer can escape, Rja3 a W9\n", cub3d_data);
			}
			j++;
		}
		i++;
	}
}

void        content_is_valid(t_cub3d **cub3d_data)
{
	size_t i;
	t_bool    player;
	t_bool    spaces;

	i = 0;
	player = false;
	spaces = false;
	while ((*cub3d_data)->map.map[i] != NULL)
	{
		check_content(cub3d_data, &player, &spaces, i);
		i++;
	}
	if (player == false || spaces == false)
		err_exit("Error\nNo player position or no spaces\n", cub3d_data);
}

void    check_content(t_cub3d **cub3d_data, t_bool *player,
			t_bool *spaces, size_t i)
{
	size_t  j;

	j = 0;
	while ((*cub3d_data)->map.map[i][j] != '\0')
	{
		if ((*cub3d_data)->map.map[i][j] == 'N' || (*cub3d_data)->map.map[i][j] == 'S' 
			|| (*cub3d_data)->map.map[i][j] == 'W' || (*cub3d_data)->map.map[i][j] == 'E')
		{
			if (*player == true)
			err_exit("Error\nMultiple player position\n", cub3d_data);
			*player = true;
			(*cub3d_data)->map.player_x = j;
			(*cub3d_data)->map.player_y = i;
			(*cub3d_data)->player.player_direction = ((*cub3d_data)->map.map[i][j]);
		}
		else if ((*cub3d_data)->map.map[i][j] == '0')
			*spaces = true;
		j++;
	}
}
