#ifndef CUB3D_H
#define CUB3D_H

#include "../lib/libft/libft.h"
#include <fcntl.h>
# include <errno.h>
# include <mlx.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>
# include <X11/Xlib.h>

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	int		player_x;
	int		player_y;
	char	**map;
	char	**tmp_map;
	int		map_height;
	int		map_width;
}			t_data;

void	draw_player(t_data *data);
void	draw_game(t_data *data);

/* key hook events */
int		ft_close(t_data *data);
int		key_hook(int keysym, t_data *data);
void	mlx_hooks_loop(t_data *data);

/* utils1 */
void	get_map_resolution(t_data *data);

#define KEY_UP 65362
#define KEY_DOWN 65364
#define KEY_LEFT 65361
#define KEY_RIGHT 65363
#define KEY_ESC 65307

#define DISPLAY_H 1000
#define DISPLAY_W 1000

#define PIXEL_W 40
#define PIXEL_H	40 

/* colors in hexa */

#define COLOR_RED      0xFF0000
#define COLOR_GREEN    0x00FF00
#define COLOR_BLUE     0x0000FF
#define COLOR_YELLOW   0xFFFF00
#define COLOR_CYAN     0x00FFFF
#define COLOR_MAGENTA  0xFF00FF
#define COLOR_ORANGE   0xFFA500
#define COLOR_PURPLE   0x800080
#define COLOR_WHITE    0xFFFFFF
#define COLOR_BLACK    0x000000
#define COLOR_GRAY     0x808080
#define COLOR_PINK     0xFFC0CB

#endif