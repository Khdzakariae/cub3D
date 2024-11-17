#include "main.h"

bool	check_cub_extension(char *file)
{
	while (file && *file)
	{
		if (*file == '.' && *(file + 1) == 'c' && *(file + 2) == 'u' && *(file + 3) == 'b' && *(file + 4) == '\0')
			return (true);
		file++;
	}
	return (false);
}
void	init_cub3d_data(t_cub3d **cub3d)
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
	(*cub3d)->player_x = -1;
	(*cub3d)->player_y = -1;
}
