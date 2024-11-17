#include "main.h"

void		load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void		fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
void		copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i);
void        map_is_valid(t_cub3d **cub3d_data);
void        content_is_valid(t_cub3d **cub3d_data);
void        fetch_map_width(t_cub3d **cub3d_data);

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
			// map contains all infos, it's surronded by walls.
			map_is_valid(cub3d_data);
			fetch_map_width(cub3d_data);
		}
		else if (cube_file[i][0] == '1' && map == true)
		{
			printf("Error\nInvalid map\n");
			// have to free what needs to be freed. TODO.
			exit(EXIT_FAILURE);
		}
		i++;
	}
    printf("Map width: %d\n", (*cub3d_data)->map.width);
    printf("Map height: %d\n", (*cub3d_data)->map.height);
    // print map
    for (size_t i = 0; i < (*cub3d_data)->map.height; i++)
        printf("%s\n", (*cub3d_data)->map.map[i]);
    if ((*cub3d_data)->map.width == 0 || (*cub3d_data)->map.height == 0)
    {
        printf("Error\nNo map\n");
        // TODO free what needs to be freed.
        exit(EXIT_FAILURE);
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

void    map_is_valid(t_cub3d **cub3d_data)
{
    // check if the map is surrounded by walls.
    size_t i;
    int     len_line;

    i = 0;
    while ((*cub3d_data)->map.map[i] != NULL)
    {
        len_line = ft_strlen((*cub3d_data)->map.map[i]);
        if (len_line < 3)
        {
            printf("Error\nMap is not surrounded by walls\n");
            // TODO free what needs to be freed.
            exit(EXIT_FAILURE);
        }
        if (i == 0)
        {
            if (check_if_all_walls((*cub3d_data)->map.map[i]) == false)
            {
                printf("Error\nMap is not surrounded by walls\n");
                // TODO free what needs to be freed.
                exit(EXIT_FAILURE);
            }
        }
        else if ((*cub3d_data)->map.map[i][0] != '1' || (*cub3d_data)->map.map[i][len_line - 1] != '1')
        {
            printf("Error\nMap is not surrounded by walls\n");
            // TODO free what needs to be freed.
            exit(EXIT_FAILURE);
        }
        else if (i == (*cub3d_data)->map.height - 1)
        {
            if (check_if_all_walls((*cub3d_data)->map.map[i]) == false)
            {
                printf("Error\nMap is not surrounded by walls\n");
                // TODO free what needs to be freed.
                exit(EXIT_FAILURE);
            }
        }
        i++;
    }
    // check if map's data is valid (0, 1, N, S, W, E).
    // check if the map content is valid.
    content_is_valid(cub3d_data);
}

void        content_is_valid(t_cub3d **cub3d_data)
{
    size_t i;
    size_t j;
    bool    player;
    bool    spaces;

    i = 0;
    player = false;
    spaces = false;
    // print map
    for (size_t i = 0; i < (*cub3d_data)->map.height; i++)
        printf("%s\n", (*cub3d_data)->map.map[i]);
    while ((*cub3d_data)->map.map[i] != NULL)
    {
        j = 0;
        while ((*cub3d_data)->map.map[i][j] != '\0')
        {
            if ((*cub3d_data)->map.map[i][j] == 'N' || (*cub3d_data)->map.map[i][j] == 'S' 
                || (*cub3d_data)->map.map[i][j] == 'W' || (*cub3d_data)->map.map[i][j] == 'E')
            {
                if (player == true)
                {
                    printf("Error\nMultiple player position\n");
                    // TODO free what needs to be freed.
                    exit(EXIT_FAILURE);
                }
                player = true;
                // player position
                (*cub3d_data)->player_x = j;
                (*cub3d_data)->player_y = i;
            }
            else if ((*cub3d_data)->map.map[i][j] == '0')
                spaces = true;
            j++;
        }
        i++;
    }
    if (player == false || spaces == false)
    {
        printf("Error\nNo player position or no spaces\n");
        // TODO free what needs to be freed.
        exit(EXIT_FAILURE);
    }
}

void    fetch_map_width(t_cub3d **cub3d_data)
{
    size_t i;
    int     max_len;

    max_len = 0;
    i = 0;
    while ((*cub3d_data)->map.map[i] != NULL)
    {
        if (ft_strlen((*cub3d_data)->map.map[i]) > max_len)
            max_len = ft_strlen((*cub3d_data)->map.map[i]);
        i++;
    }
    (*cub3d_data)->map.width = max_len;
}
