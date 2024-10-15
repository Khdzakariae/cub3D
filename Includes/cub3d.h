#ifndef CUB3D_H
#define CUB3D_H

#include "../lib/libft/libft.h"
#include <fcntl.h>
# include <errno.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <X11/Xlib.h>

typedef struct s_data
{
	void	*mlx;
	void	*win;
	int		player_x;
	int		player_y;
	char	**map;
	char	**tmp_map;
}			t_data;

void	draw_player(t_data *data);
void	draw_game(t_data *data);

/* key hook events */
int		ft_close(t_data *data);
int		key_hook(int keysym, t_data *data);
void	mlx_hooks_loop(t_data *data);

#define KEY_UP 65362
#define KEY_DOWN 65364
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define KEY_ESC 65307

#define HEIGHT 1200
#define WIDTH 1200

#define width_pixel  5
#define height_pixel  5


#endif