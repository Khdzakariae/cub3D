#include "../Includes/cub3d.h"

void	init_game(t_data *data);
int		read_map(t_data *data, char *map);
void	render_background(t_data *data, int color, int x, int y);
void	draw_player(t_data *data, bool flage);
void	draw_game(t_data *data);

int main(int ac, char **av)
{
	t_data data;
	if (ac > 1)
	{
		if (read_map(&data, av[1]) == 0)
			return (0);
		for (int i = 0; data.map[i] != NULL; i++)
		{
			for (int j = 0; data.map[i][j] != '\0'; j++)
			{
				if (data.map[i][j] == 'P')
				{
					data.player_x = j;
					data.player_y = i;
				}
			}
		}


		get_map_resolution(&data);
		init_game(&data);
        draw_player(&data, false);
	    mlx_key_hook(data.win, key_hook, &data);
	    mlx_loop(data.mlx);


	}
	return (0);
}


void	init_game(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return ;
	data->win = mlx_new_window(data->mlx, data->map_width * PIXEL_W,
			data->map_height * PIXEL_H, "CUB3D");
	if (!data->win)
		return ;
}

int	read_map(t_data *data, char *map)
{
	int		k;
	char	*s;
	char	*ret;

	ret = NULL;
	k = open(map, O_RDONLY);
	if (k == -1)
		return (write(1, "ENTRER MAP VALID !", 24), 0);
	s = get_next_line(k);
	if (!s)
		return (write(1,  "FILLE VIDE !!", 20), 0);
	while (s)
	{
		ret = ft_strjoin(ret, s);
		free(s);
		s = get_next_line(k);
	}
	data->map = ft_split(ret, 10);
	data->tmp_map = ft_split(ret, 10);
	free(ret);

	return (close(k), 1);
}

void render_background(t_data *data, int color, int x, int y)
{
	int offset_x = 0;
	int offset_y = 0;
	int tmp = 0;
	if (color == COLOR_BLUE)
		tmp = 20;
	for (int i = 0; i < PIXEL_H -tmp; ++i)
	{
		for (int j = 0; j < PIXEL_W - tmp; ++j)
		{
			mlx_pixel_put(data->mlx, data->win, x + j + offset_x, y + i + offset_y, color);
		}
	}
	tmp = 0;
}

void draw_player(t_data *data, bool flage)
{
    if (flage == true)
    {
        puts("ana dkhalt hna");
		render_background(data, COLOR_BLACK, data->old_player_x *PIXEL_H , data->old_player_y * PIXEL_W);
		render_background(data, COLOR_BLUE, data->player_x *PIXEL_H , data->player_y * PIXEL_W);
        return;

    }
	int	x;
	int	y;

	for (int i = 0; data->map[i] != NULL; ++i)
	{
		for (int j = 0; data->map[i][j]; ++j)
		{
			x = j * PIXEL_H;
			y = i * PIXEL_W; 
			if (data->map[i][j] == '0')
				render_background(data, COLOR_BLACK, x, y);
			else if (data->map[i][j] == '1')
				render_background(data, COLOR_RED, x, y);
			else if (data->map[i][j] == 'P')
			{
				data->map[i][j] = '0';
				render_background(data, COLOR_BLUE, x, y);
			}
		}
	}
	render_background(data, COLOR_BLUE, data->player_x * PIXEL_W, data->player_y * PIXEL_H);
}

// void draw_game(t_data *data)
// {
// 	draw_player(data, ); 
// }
