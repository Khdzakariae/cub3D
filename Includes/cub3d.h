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

typedef struct s_data
{
	void	*mlx;
	void	*win;
    int     player_x;
    int     player_y;
	char	**map;
	char	**tmp_map;
}			t_data;

void draw_player(t_data *data);
void draw_game(t_data *data);
int	handle_destroy_notify(t_data *data);
int	ButtonRelease(int keysym, t_data *data);

#define KEY_UP 126
#define KEY_DOWN 125
#define KEY_LEFT 123
#define KEY_RIGHT 124

#define HEIGHT 720
#define WIDTH 1080

#define img_width WIDTH / 40
#define img_height HEIGHT / 40



#endif