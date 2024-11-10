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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 70
#define MAP_NUM_ROWS 11
#define MAP_NUM_COLS 15
#define WINDOW_WIDTH (MAP_NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)
#define FOV_ANGLE (60 * (M_PI / 180))
#define NUM_RAYS WINDOW_WIDTH

#define KEY_UP 126
#define KEY_DOWN 125
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_ESC 53

#define TRUE 1
#define FALSE 0

typedef struct s_map {
    int map_height;
    int map_width;
    char **grid;
} t_map;

typedef struct s_player {
    double x;
    double y;
    double radius;
    double turnDirection;
    double walkDirection;
    double rotationAngle;
    double moveSpeed;
    double rotationSpeed;
} t_player;


struct Ray {
    float rayAngle;
    float wallHitX;
    float wallHitY;
    float distance;
    int wasHitVertical;
    int isRayFacingUp;
    int isRayFacingDown;
    int isRayFacingLeft;
    int isRayFacingRight;
    int wallHitContent;
} rays[NUM_RAYS];

typedef struct s_data {
    void *mlx;
    void *win;
    t_map *map;
    t_player *player;
    struct Ray rays[NUM_RAYS];
} t_data;
# endif
