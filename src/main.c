#include "cub3d.h"

void	init_game(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return ;
	data->win = mlx_new_window(data->mlx, 640,
			870, "SO_LONG");
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

int main(int ac, char **av)
{
    t_data	data;

    if (ac > 1)
    {
        if (read_map(&data, av[1]) == 0)
                return (0);
        print_map(data.tmp_map);
        init_game(&data);
        mlx_loop(data.mlx);

    }
    return (0);
}
