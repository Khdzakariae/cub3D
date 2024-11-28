#ifndef CUB3D_H
# define CUB3D_H

# include "../lib/libft/libft.h"
#include "../lib/mlx/mlx.h"
#include "parsing.h"
# include <fcntl.h>
# include <errno.h>
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


#define MOUSE_SENSITIVITY 1
#define MINIMAP_SCALE_FACTOR 0.2
#define MINIMAP_PLAYER_COLOR 0xFF0000 // Red color for the player
#define MINIMAP_WALL_COLOR 0xFFFFFF // White color for walls
#define MINIMAP_EMPTY_COLOR 0x000000 // Black color for empty spaces

# define COLOR_RED      0xFF0000
# define COLOR_GREEN    0x00FF00
# define COLOR_BLUE     0x0000FF
# define COLOR_YELLOW   0xFFFF00
# define COLOR_CYAN     0x00FFFF
# define COLOR_MAGENTA  0xFF00FF
# define COLOR_ORANGE   0xFFA500
# define COLOR_PURPLE   0x800080
# define COLOR_WHITE    0xFFFFFF
# define COLOR_BLACK    0x000000
# define COLOR_GRAY     0x808080
# define COLOR_PINK     0xFFC0CB

#define NUM_RAYS WINDOW_WIDTH
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define FOV_ANGLE (60 * (M_PI / 180))

// #define TILE_SIZE 256

#define CIEL_IMAGE TRUE
#define DARKNESS 500
#define RADIUS 30
#define COLLISION_MARGIN 50
#define KEY_ACTIVE_MOUSSE 65436

//  #define KEY_UP 126
// #define KEY_DOWN 125
// #define KEY_LEFT 123
// #define KEY_RIGHT 124
// #define KEY_ESC 53 

#define KEY_W 119
#define KEY_S 115
#define KEY_A 97
#define KEY_D 100



#define TRUE 1
#define FALSE 0

#define CIEL "texters/9078_1.xpm"

typedef struct s_wall
{
    double wallX;
    int texture_id;
    int wallTop;
    int wallBottom;
    int wallHeight;
    float perpDistance;
}t_wall;

typedef struct s_img
{
    void    *img_ptr;
    char    *image_pixel_ptr;
    int     bits_per_pixel;
    int     endian;
    int     line_len;
} t_img;

typedef struct s_ray {
    char c;
    float   rayAngle;
    float   wallHitX;
    float   wallHitY;
    float   distance;
    int     wasHitVertical;
    int     isRayFacingUp;
    int     isRayFacingDown;
    int     isRayFacingLeft;
    int     isRayFacingRight;
    int     wallHitContent;
} t_ray;

typedef struct s_data {
    t_wall *wall;
	int flage_mousse;
    t_cub3d     game;
    void        *mlx;
    void        *win;
    t_img       img;
    t_ray       rays[NUM_RAYS];
} t_data;

 #define KEY_UP 65362
 #define KEY_DOWN 65364
 #define KEY_LEFT 65361
 #define KEY_RIGHT 65363
 #define KEY_ESC 65307


void cleanup_textures(t_cub3d *game);
int cleanup(t_data *data);

int update_player(t_player *player, t_map *map);
int game_loop(t_data *data);

void set_player_orientation(t_player *player, char orientation) ;
void init_player(t_data *data);
void init_textures(t_data *data);
void init_game(t_data *data);
bool init(t_data *data);

int key_press(int keycode, t_data *data);
int key_release(int keycode, t_data *data);
int mouse_move(int x, int y, t_data *data);

double distanceBetweenPoints(double x1, double y1, double x2, double y2);
double normalizeAngle(double angle);
int is_wall_at(t_map *map, double x, double y);

int is_within_bounds(t_map *map, double x, double y);
int has_wall_at(t_map *map, double x, double y);
int unit_circle(float angle, char c);
int inter_check(t_data *data, float angle, float *inter, float *step, int is_horizon);
int wall_hit(float x, float y, t_data *data);

float get_h_inter(t_data *data, float angle);
float get_v_inter(t_data *data, float angle);
void castRay(t_data *data, float rayAngle, int stripId);
void castAllRays(t_data *data);

void my_pixel_put(t_img *img, int x, int y, int color);
int apply_darkness(int texColor, float darkness);
void ciel(t_data *data);
void drawing_east(t_data *data);
void drawing_floor(t_data *data);

int create_trgb(int t, int r, int g, int b);
int my_mlx_pixel_get(int flag, t_data *data, double wallX, int y, int wallHeight) ;
double get_wallX(t_data *data, int i, float perpDistance);
void render_wall_texture(t_data *data, int i);

int get_wall_top(int wallHeight);
int get_wall_bottom(int wallHeight);
int get_wall_height(float perpDistance);
int get_texture_id(t_data *data, int i);
void render_walls(t_data *data);

void    render_player(t_data *data);
int	update_frame(t_data *data);

# endif
