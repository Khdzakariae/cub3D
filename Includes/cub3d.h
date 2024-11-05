#ifndef CUB3D_H
# define CUB3D_H

# include "../lib/libft/libft.h"
# include <fcntl.h>
# include <errno.h>
#include "../mlx/mlx.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
#include <stdbool.h>
// # include <X11/Xlib.h>

/* keysyms */
// #define PI 3.1415926535
// #define FOV_ANGLE 60 * (PI / 180)
// #define NUM_RAYS 320;
// #define RAY_ANGLE += FOV_ANGLE / NUM_RAYS


// # define KEY_UP 126
// # define KEY_DOWN 125
// # define KEY_LEFT 123
// # define KEY_RIGHT 124
// # define KEY_ESC 65307

// # define DISPLAY_H 720
// # define DISPLAY_W 680

// # define PIXEL_W 32
// # define PIXEL_H	32
// # define SIZE_PLAYER 0.1
// /* colors in hexa */
// # define COLOR_RED      0xFF0000
// # define COLOR_GREEN    0x00FF00
// # define COLOR_BLUE     0x0000FF
// # define COLOR_YELLOW   0xFFFF00
// # define COLOR_CYAN     0x00FFFF
// # define COLOR_MAGENTA  0xFF00FF
// # define COLOR_ORANGE   0xFFA500
// # define COLOR_PURPLE   0x800080
// # define COLOR_WHITE    0xFFFFFF
// # define COLOR_BLACK    0x000000
// # define COLOR_GRAY     0x808080
// # define COLOR_PINK     0xFFC0CB

// #include <math.h>
// typedef struct s_data
// {
// 	void	*mlx;
// 	void	*win;
// 	double		old_player_x;
// 	double		old_player_y;
// 	double		player_x;
// 	double		player_y;

// 	double  turnderection;
// 	double	walkderiction;
// 	double  rotationangle;
// 	double  movespeed;
// 	double  retationspeed;
// 	char	**map;
// 	char	**tmp_map;
// 	int		map_height;
// 	int		map_width;
// }	t_data;

// void	draw_player(t_data *data, bool flag);
// void	draw_game(t_data *data);

// /* key hook events */
// int key_release(int keysym, t_data *data);
// int		ft_close(t_data *data);
// int		key_hook(int keysym, t_data *data);
// void	mlx_hooks_loop(t_data *data);

// /* utils1 */
// void	get_map_resolution(t_data *data);

# endif /* CUB3D_H */
