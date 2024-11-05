#include "../Includes/cub3d.h"

void	mlx_hooks_loop(t_data *data);
int		key_hook(int keysym, t_data *data);
int		ft_close(t_data *data);

void	mlx_hooks_loop(t_data *data)
{
	mlx_key_hook(data->win, key_hook, data);
	// mlx_hook(data->win, DestroyNotify, StructureNotifyMask, &ft_close, data);
	mlx_loop(data->mlx);
}

// int	key_hook(int keysym, t_data *data)
// {
// 	if (keysym == KEY_ESC)
// 	{
// 		mlx_destroy_window(data->mlx, data->win);
// 		// mlx_destroy_display(data->mlx);
// 		free(data->mlx);
// 		exit(0);
// 	}
// 	if (keysym == KEY_UP){
// 		data.walk
// 	}
// 	else if (keysym == KEY_DOWN){

// 	}
// 	else if (keysym == KEY_LEFT){
// 	}
// 	else if (keysym == KEY_RIGHT){
// 	}

// }

int	key_hook(int keysym, t_data *data)
{
	printf("the key  is %d \n", keysym);
	if (keysym == KEY_ESC)
	{
		mlx_destroy_window(data->mlx, data->win);
		// mlx_destroy_display(data->mlx);
		free(data->mlx);
		exit(0);
	}
	if (keysym == KEY_UP)
	{
		if (data->map[(int)data->player_y - 1][(int)data->player_x] != '1'){
			data->old_player_x = data->player_x;
			data->old_player_y = data->player_y;
			data->player_y -= SIZE_PLAYER;
		}
	}
	else if (keysym == KEY_DOWN)
	{
		if (data->map[(int)data->player_y + 1][(int)data->player_x] != '1'){
			data->old_player_x = data->player_x;
			data->old_player_y = data->player_y;
			data->player_y += SIZE_PLAYER;
		}
	}
	else if (keysym == KEY_LEFT)
	{
		if (data->map[(int)data->player_y][(int)data->player_x - 1] != '1'){
			data->old_player_x = data->player_x;
			data->old_player_y = data->player_y;
			data->player_x -= SIZE_PLAYER;
		}
	}
	else if (keysym == KEY_RIGHT)
	{
		if (data->map[(int)data->player_y][(int)data->player_x + 1] != '1'){
			data->old_player_x = data->player_x;
			data->old_player_y = data->player_y;
			data->player_x += SIZE_PLAYER ;
		}
	}
	draw_player(data, true);
	return (0);
}

int	ft_close(t_data *data)
{
	mlx_destroy_window(data->mlx, data->win);
	// mlx_destroy_display(data->mlx);
	free(data->mlx);
	exit(0);
}
