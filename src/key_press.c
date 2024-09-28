
#include "cub3d.h"

int	handle_destroy_notify(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	// mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

int	key_hook(int keysym, t_data *data)
{
	if (keysym == 65307)
	{
		mlx_destroy_window(data->mlx, data->win);
		// mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(1);
	}
	return (0);
}