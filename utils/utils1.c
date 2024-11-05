#include "../Includes/cub3d.h"

void	get_map_resolution(t_data *data)
{
	data->map_height = 0;
	data->map_width = 0;
	for (int i = 0; data->map[i] != NULL; i++)
	{
		data->map_height++;
		int row_length = ft_strlen(data->map[i]);
		if (row_length > data->map_width)
			data->map_width = row_length;
	}
}