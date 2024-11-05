#include "../Includes/cub3d.h"

void	init_game(t_data *data);
int		read_map(t_data *data, char *map);
void	render_background(t_data *data, int color, int x, int y);
void	draw_player(t_data *data, bool flage);
void	draw_game(t_data *data);

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

void ft_draw_line(t_data *data, int x1, int y1, int x2, int y2, int color)
{
	printf("the cordonner of x :%d\n", x1);
	printf("the cordonner of y : %d\n", x2);

    int step;
    int x;
    int y;
    int i;
    int delta_x;
    int delta_y;
    
    delta_x = x2 - x1;
    delta_y = y2 - y1;
    if (abs(delta_x) >= abs(delta_y))
        step = abs(delta_x);
    else
        step = abs(delta_y);
    delta_x = delta_x / step;
    delta_y = delta_y / step;
    x = x1;
    y = x2;
    i = 0;
    while (i < step)
    {
        mlx_pixel_put(data->mlx,data->win, x, y, color);
        x += delta_x;
        y += delta_y;
        i++;
    }
}



void	init_game(t_data *data)
{
	data->turnderection = 0;
	data->walkderiction = 0;
	data->rotationangle = PI / 2;
	data->movespeed = 3.0;
	data->retationspeed = 3 * (PI / 180);

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
	for (int i = 0; i < PIXEL_H ; ++i)
	{
		for (int j = 0; j < PIXEL_W ; ++j)
		{
			mlx_pixel_put(data->mlx, data->win, x + j + offset_x, y + i + offset_y, color);
		}
	}
}

void draw_circle(t_data *data, int center_x, int center_y, int radius, int color)
{
    int x, y;
    int r_squared = radius * radius;

    for (y = center_y - radius; y <= center_y + radius; y++)
    {
        for (x = center_x - radius; x <= center_x + radius; x++)
        {
            if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) <= r_squared)
            {
                mlx_pixel_put(data->mlx, data->win, x, y, color);
            }
        }
    }
}

void draw_player(t_data *data, bool flag)
{
    int player_center_x = data->player_x * PIXEL_W + PIXEL_W / 2;
    int player_center_y = data->player_y * PIXEL_H + PIXEL_H / 2;
    int radius = PIXEL_W / 4; 

    if (flag == true)
    {
        draw_circle(data, data->old_player_x * PIXEL_W + PIXEL_W / 2, 
                    data->old_player_y * PIXEL_H + PIXEL_H / 2, radius, COLOR_BLACK);
        draw_circle(data, player_center_x, player_center_y, radius, COLOR_BLUE);

    int line_start_x = data->player_x * PIXEL_W + PIXEL_W / 2;
    int line_start_y = data->player_y * PIXEL_W + (PIXEL_W * 2); 
        ft_draw_line(data, 
                     line_start_x, line_start_y,
                     player_center_x + cos(data->rotationangle) * 20,
                     player_center_y + sin(data->rotationangle) * 20, 
                     COLOR_YELLOW);
        return;
    }

    int x, y;
    for (int i = 0; data->map[i] != NULL; ++i)
    {
        for (int j = 0; data->map[i][j]; ++j)
        {
            x = j * PIXEL_W;
            y = i * PIXEL_H;

            if (data->map[i][j] == '0')
                render_background(data, COLOR_BLACK, x, y);
            else if (data->map[i][j] == '1')
                render_background(data, COLOR_RED, x, y);
            else if (data->map[i][j] == 'P')
            {
                data->map[i][j] = '0';
                draw_circle(data, player_center_x, player_center_y, radius, COLOR_BLUE);
            }
        }
    }

    int line_start_x = data->player_x * PIXEL_W + PIXEL_W / 2;
    int line_start_y = data->player_y * PIXEL_W + (PIXEL_W * 2); 

    ft_draw_line(data, 
                 line_start_x, line_start_y, 
                 player_center_x + cos(data->rotationangle) * 20, 
                 player_center_y + sin(data->rotationangle) * 20,
                 COLOR_YELLOW);
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


		get_map_resolution(&data);
		init_game(&data);
        draw_player(&data, false);
	    mlx_key_hook(data.win, key_hook, &data);
		// mlx_hook(data.win, 3, 1L << 1, key_release, &data);
	    mlx_loop(data.mlx);


	}
	return (0);
}
