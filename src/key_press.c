
#include "cub3d.h"

int	handle_destroy_notify(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	// mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}

int	ButtonRelease(int keysym, t_data *data)
{
	if (keysym == 53)
	{
		mlx_destroy_window(data->mlx, data->win);
		// mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(0);
	}
	if (keysym == KEY_UP)
    {
        if (data->map[data->player_y - 1][data->player_x] != '1')
            data->player_y--;
    }
    else if (keysym == KEY_DOWN)
    {
        if (data->map[data->player_y + 1][data->player_x] != '1')
            data->player_y++;
    }
    else if (keysym == KEY_LEFT)
    {
        if (data->map[data->player_y][data->player_x - 1] != '1')
            data->player_x--;
    }
    else if (keysym == KEY_RIGHT)
    {
        if (data->map[data->player_y][data->player_x + 1] != '1')
            data->player_x++;
    }
    draw_game(data);
	return (0);
}