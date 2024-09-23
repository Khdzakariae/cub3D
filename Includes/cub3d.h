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
	int		player_x;
	int		player_y;
	char	**map;
	char	**tmp_map;
	void	*sprint;
}			t_data;


#endif