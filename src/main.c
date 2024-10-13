#include "../Includes/cub3d.h"


void	init_game(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return ;
	data->win = mlx_new_window(data->mlx, WIDTH,
			HEIGHT, "CUB3D");
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
    if (data->win == NULL)
        return;

    for (int i = 0; i < width_pixel; ++i)
    {
        for (int j = 0; j < height_pixel; ++j)
        {
            mlx_pixel_put(data->mlx, data->win, x + j, y + i, color);
        }
    }
}

void draw_player(t_data *data)
{
    for (int i = 0; data->map[i] != NULL; ++i)
    {
        for (int j = 0; data->map[i][j] != '\0'; ++j)
        {
            int x = j * width_pixel; 
            int y = i * height_pixel; 

            if (data->map[i][j] == '0')
                render_background(data, 0x00FF00, x, y);
            else if (data->map[i][j] == '1')
                render_background(data, 0xFF0000, x, y);
            else if (data->map[i][j] == 'P')
                render_background(data, 0x0000FF, x, y);
        }
    }
    render_background(data, 0x0000FF, data->player_x * height_pixel, data->player_y * height_pixel);
}

void draw_game(t_data *data)
{
    render_background(data, 0xFFFFFF, data->player_x, data->player_y);
    draw_player(data); 
}


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
        init_game(&data);
        mlx_key_hook(data.win, ButtonRelease, &data);
        mlx_hook(data.win, 17, 0, handle_destroy_notify, &data);
        draw_game(&data);
        mlx_loop(data.mlx);
    }
    return (0);
}
