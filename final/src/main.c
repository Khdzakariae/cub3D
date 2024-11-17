#include "../Includes/cub3d.h"

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define NUM_RAYS WINDOW_WIDTH
#define FOV_ANGLE (60 * (M_PI / 180))
#define TILE_SIZE 64

#define KEY_UP 126
#define KEY_DOWN 125
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_ESC 53

#define TRUE 1
#define FALSE 0

typedef struct s_img
{
    void    *img_ptr;
    char    *image_pixel_ptr;
    int     bits_per_pixel;
    int     endian;
    int     line_len;
} t_img;

typedef struct s_map {
    int     map_height;
    int     map_width;
    char    **grid;
    int     tile_size;
} t_map;

typedef struct s_player {
    double  x;
    double  y;
    double  radius;
    double  turnDirection;
    double  walkDirection;
    double  rotationAngle;
    double  moveSpeed;
    double  rotationSpeed;
} t_player;

typedef struct s_ray {
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
    void        *mlx;
    void        *win;
    t_img       img;
    t_map       *map;
    t_player    *player;
    t_ray       rays[NUM_RAYS];
} t_data;


void    cleanup_map(t_map *map);
int     close_window(t_data *data);
char    *ft_strjoin(char *s1, char *s2);
char    *get_next_line(int fd);

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle;
}
int close_window(t_data *data)
{
    if (!data)
        return -1;

    if (data->map)
        cleanup_map(data->map);
    if (data->win)
        mlx_destroy_window(data->mlx, data->win);
    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr);
    exit(0);
    return 0;
}

void safe_free(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

void cleanup_map(t_map *map)
{
    if (!map || !map->grid)
        return;

    for (int i = 0; map->grid[i]; i++)
        safe_free((void**)&map->grid[i]);
    safe_free((void**)&map->grid);
}

void my_pixel_put(t_img *img, int x, int y, int color)
{
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        int offset = (y * img->line_len) + (x * (img->bits_per_pixel / 8));
        *((unsigned int *)(img->image_pixel_ptr + offset)) = color;
    }
}

int create_trgb(int t, int r, int g, int b)
{
    return (t << 24) | (r << 16) | (g << 8) | b;
}


double distanceBetweenPoints(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int has_wall_at(t_map *map, double x, double y)
{
    if (!map || !map->grid || x < 0 || x >= map->map_width * map->tile_size 
        || y < 0 || y >= map->map_height * map->tile_size)
        return 1;

    int mapX = (int)(x / map->tile_size);
    int mapY = (int)(y / map->tile_size);

    return (map->grid[mapY][mapX] == '1');
}

void get_map_resolution(t_map *map)
{
    if (!map || !map->grid)
        return;

    map->map_height = 0;
    map->map_width = 0;
    for (int i = 0; map->grid[i] != NULL; i++)
    {
        map->map_height++;
        int row_length = strlen(map->grid[i]);
        if (row_length > map->map_width)
            map->map_width = row_length;
    }
}

int update_player(t_player *player, t_map *map)
{
    if (!player || !map)
        return -1;

    player->rotationAngle = normalizeAngle(player->rotationAngle + 
                           player->turnDirection * player->rotationSpeed);

    double moveStep = player->walkDirection * player->moveSpeed;
    double newPlayerX = player->x + cos(player->rotationAngle) * moveStep;
    double newPlayerY = player->y + sin(player->rotationAngle) * moveStep;

    if (!has_wall_at(map, newPlayerX, newPlayerY))
    {
        player->x = newPlayerX;
        player->y = newPlayerY;
    }
    return 0;
}

int key_press(int keycode, t_data *data)
{
    if (!data || !data->player)
        return -1;

    if (keycode == KEY_UP)
        data->player->walkDirection = 1;
    else if (keycode == KEY_DOWN)
        data->player->walkDirection = -1;
    else if (keycode == KEY_LEFT)
        data->player->turnDirection = -1;
    else if (keycode == KEY_RIGHT)
        data->player->turnDirection = 1;
    else if (keycode == KEY_ESC)
        close_window(data);
    return 0;
}

int key_release(int keycode, t_data *data)
{
    if (!data || !data->player)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_DOWN)
        data->player->walkDirection = 0;
    else if (keycode == KEY_LEFT || keycode == KEY_RIGHT)
        data->player->turnDirection = 0;
    return 0;
}

int read_map(t_data *data, char *map_path)
{
    int fd = open(map_path, O_RDONLY);
    if (fd == -1)
        return 0;

    char *line;
    char *content = NULL;
    char *temp;

    while ((line = get_next_line(fd)))
    {
        temp = ft_strjoin(content, line);
        // Don't free content here as ft_strjoin already freed it
        free(line);
        content = temp;
        if (!content)
            break;
    }

    if (!content)
    {
        close(fd);
        return 0;
    }

    data->map->grid = ft_split(content, '\n');
    free(content);  // Free the final content
    close(fd);

    if (!data->map->grid)
        return 0;

    get_map_resolution(data->map);
    data->map->tile_size = TILE_SIZE;
    return 1;
}


void castRay(t_data *data, float rayAngle, int stripId) {
    rayAngle = normalizeAngle(rayAngle);

    int isRayFacingDown = rayAngle > 0 && rayAngle < M_PI;
    int isRayFacingUp = !isRayFacingDown;
    int isRayFacingRight = rayAngle < 0.5 * M_PI || rayAngle > 1.5 * M_PI;
    int isRayFacingLeft = !isRayFacingRight;

    float xintercept, yintercept;
    float xstep, ystep;

    int foundHorzWallHit = FALSE;
    float horzWallHitX = 0;
    float horzWallHitY = 0;

    yintercept = floor(data->player->y / data->map->tile_size) * data->map->tile_size;
    yintercept += isRayFacingDown ? data->map->tile_size : 0;
    xintercept = data->player->x + (yintercept - data->player->y) / tan(rayAngle);
    ystep = data->map->tile_size;
    ystep *= isRayFacingUp ? -1 : 1;
    xstep = data->map->tile_size / tan(rayAngle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    while (nextHorzTouchX >= 0 && nextHorzTouchX <= data->map->map_width * data->map->tile_size &&
           nextHorzTouchY >= 0 && nextHorzTouchY <= data->map->map_height * data->map->tile_size) {
        if (has_wall_at(data->map, nextHorzTouchX, nextHorzTouchY)) {
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            foundHorzWallHit = TRUE;
            break;
        }
        nextHorzTouchX += xstep;
        nextHorzTouchY += ystep;
    }

    int foundVertWallHit = FALSE;
    float vertWallHitX = 0;
    float vertWallHitY = 0;

    xintercept = floor(data->player->x / data->map->tile_size) * data->map->tile_size;
    xintercept += isRayFacingRight ? data->map->tile_size : 0;
    yintercept = data->player->y + (xintercept - data->player->x) * tan(rayAngle);
    xstep = data->map->tile_size;
    xstep *= isRayFacingLeft ? -1 : 1;
    ystep = data->map->tile_size * tan(rayAngle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

    while (nextVertTouchX >= 0 && nextVertTouchX <= data->map->map_width * data->map->tile_size &&
           nextVertTouchY >= 0 && nextVertTouchY <= data->map->map_height * data->map->tile_size) {
        if (has_wall_at(data->map, nextVertTouchX, nextVertTouchY)) {
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            foundVertWallHit = TRUE;
            break;
        }
        nextVertTouchX += xstep;
        nextVertTouchY += ystep;
    }

    double horzDistance = foundHorzWallHit ? distanceBetweenPoints(data->player->x, data->player->y, horzWallHitX, horzWallHitY) : INT_MAX;
    double vertDistance = foundVertWallHit ? distanceBetweenPoints(data->player->x, data->player->y, vertWallHitX, vertWallHitY) : INT_MAX;

    if (horzDistance < vertDistance) {
        data->rays[stripId].distance = horzDistance;
        data->rays[stripId].wallHitX = horzWallHitX;
        data->rays[stripId].wallHitY = horzWallHitY;
        data->rays[stripId].wasHitVertical = FALSE;
    } else {
        data->rays[stripId].distance = vertDistance;
        data->rays[stripId].wallHitX = vertWallHitX;
        data->rays[stripId].wallHitY = vertWallHitY;
        data->rays[stripId].wasHitVertical = TRUE;
    }
}

void render_walls(t_data *data) {
    memset(data->img.image_pixel_ptr, 0, WINDOW_WIDTH * WINDOW_HEIGHT * (data->img.bits_per_pixel / 8));

    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT / 2; y++) {
            my_pixel_put(&data->img, x, y, create_trgb(0, 100, 100, 100));
        }
    }


    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y++) {
            my_pixel_put(&data->img, x, y, create_trgb(0, 50, 50, 50)); 
        }
    }

    for (int i = 0; i < NUM_RAYS; i++) {
        float distance =  data->rays[i].distance;   
        
        int wallHeight = (int)((WINDOW_HEIGHT / distance) * data->map->tile_size);
        
        int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        if (wallTop < 0) wallTop = 0;
        
        int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        if (wallBottom >= WINDOW_HEIGHT) wallBottom = WINDOW_HEIGHT - 1;

        float darknessFactor = 1.0f + (distance / (data->map->tile_size * 5));
        int baseColor = data->rays[i].wasHitVertical ? 160 : 200; 
        int adjustedColor = (int)(baseColor / darknessFactor);
        
        if (adjustedColor < 0) adjustedColor = 0;
        if (adjustedColor > 255) adjustedColor = 255;

        int wallColor = create_trgb(0, adjustedColor, adjustedColor, adjustedColor);

        for (int y = wallTop; y < wallBottom; y++) {
            my_pixel_put(&data->img, i, y, wallColor);
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}

void castAllRays(t_data *data) {
    float rayAngle = data->player->rotationAngle - (FOV_ANGLE / 2);
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        castRay(data, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

int game_loop(t_data *data) {
    update_player(data->player, data->map);
    mlx_clear_window(data->mlx, data->win);
    castAllRays(data);
    render_walls(data);
    return 0;
}

void init_game(t_data *data)
{
    if (!data || !data->map)
        return;

    data->mlx = mlx_init();
    if (!data->mlx)
        return;

    data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Raycasting");
    if (!data->win)
        return;

    data->img.img_ptr = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!data->img.img_ptr)
        return;

    data->img.image_pixel_ptr = mlx_get_data_addr(data->img.img_ptr, 
                                              &data->img.bits_per_pixel,
                                              &data->img.line_len,
                                              &data->img.endian);

    data->player = malloc(sizeof(t_player));
    if (!data->player)
        return;

    data->player->radius = 3;
    data->player->turnDirection = 0;
    data->player->walkDirection = 0;
    data->player->rotationAngle = M_PI / 2;
    data->player->moveSpeed = 3;
    data->player->rotationSpeed = 2 * (M_PI / 180);

    int startX = data->map->map_width * data->map->tile_size / 2;
    int startY = data->map->map_height * data->map->tile_size / 2;

    data->player->x = startX;
    data->player->y = startY;

    if (has_wall_at(data->map, data->player->x, data->player->y))
    {
        data->player->x = startX - data->map->tile_size;
        data->player->y = startY - data->map->tile_size;
    }
}


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;
    t_map map;

    memset(&data, 0, sizeof(t_data));
    memset(&map, 0, sizeof(t_map));
    data.map = &map;

    if (!read_map(&data, argv[1]))
    {
        fprintf(stderr, "Error: Failed to read map file\n");
        return 1;
    }
    init_game(&data);
    if (!data.mlx || !data.win || !data.player)
    {
        cleanup_map(&map);
        fprintf(stderr, "Error: Failed to initialize game\n");
        return 1;
    }

    mlx_loop_hook(data.mlx, game_loop, &data);
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_hook(data.win, 17, 0, close_window, &data);  // Handle window close button
    mlx_loop(data.mlx);

    return 0;
}