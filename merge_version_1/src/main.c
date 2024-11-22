#include "../Includes/cub3d.h"

char    **ft_split(char const *s, char c);
void    cleanup_map(t_map *map);
int     close_window(t_data *data);

char    *ft_strjoin(char *s1, char *s2);

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle;
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
    if (!map || !map->map)
        return;

    for (int i = 0; map->map[i]; i++)
        safe_free((void**)&map->map[i]);
    safe_free((void**)&map->map);
}

int close_window(t_data *data)
{
    if (!data)
        return -1;

    if (data->cub)
    {
        // Cleanup textures
        if (data->cub->textures.ea.img_ptr)
            mlx_destroy_image(data->mlx, data->cub->textures.ea.img_ptr);
        if (data->cub->textures.no.img_ptr)
            mlx_destroy_image(data->mlx, data->cub->textures.no.img_ptr);
        if (data->cub->textures.so.img_ptr)
            mlx_destroy_image(data->mlx, data->cub->textures.so.img_ptr);
        if (data->cub->textures.we.img_ptr)
            mlx_destroy_image(data->mlx, data->cub->textures.we.img_ptr);
        
        cleanup_map(&data->cub->map);
    }

    if (data->img.img_ptr)
        mlx_destroy_image(data->mlx, data->img.img_ptr);
    if (data->win)
        mlx_destroy_window(data->mlx, data->win);
    if (data->mlx)
    {
        // mlx_destroy_display(data->mlx);
        free(data->mlx);
    }
    exit(0);
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
    if (!map || !map->map || x < 0 || x >= map->width * TILE_SIZE 
        || y < 0 || y >= map->height * TILE_SIZE)
        return 1;

    int mapX = (int)(x / TILE_SIZE);
    int mapY = (int)(y /TILE_SIZE);

    return (map->map[mapY][mapX] == '1');
}



int unit_circle(float angle, char c)
{
    if (c == 'x')
    {
        if (angle > 0 && angle < M_PI)
            return (1);
    }
    else if (c == 'y')
    {
        if (angle > (M_PI / 2) && angle < (3 * M_PI) / 2)
            return (1);
    }
    return (0);
}

int inter_check(t_data *data, float angle, float *inter, float *step, int is_horizon)
{
    if (is_horizon)
    {
        if (angle > 0 && angle < M_PI)
        {
            *inter += TILE_SIZE;
            return (-1);
        }
        *step *= -1;
    }
    else
    {
        if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) 
        {
            *inter += TILE_SIZE;
            return (-1);
        }
        *step *= -1;
    }
    return (1);
}

int wall_hit(float x, float y, t_data *data)
{
    int x_m, y_m;

    if (!data || !data->cub)
        return 0;

    if (x < 0 || x >= data->cub->map.width * TILE_SIZE || 
        y < 0 || y >= data->cub->map.height * TILE_SIZE)
        return 1; 

    x_m = floor(x / TILE_SIZE);
    y_m = floor(y / TILE_SIZE);

    if (y_m >= data->cub->map.height || x_m >= data->cub->map.width)
        return 1;

    if (data->cub->map.map[y_m] && x_m < data->cub->map.width)
        return (data->cub->map.map[y_m][x_m] == '1');

    return 1;
}

float get_h_inter(t_data *data, float angle)
{
    float h_x, h_y, x_step, y_step;
    int pixel;

    y_step = TILE_SIZE;
    x_step = TILE_SIZE / tan(angle);
    h_y = floor(data->cub->player.y /TILE_SIZE) * TILE_SIZE;
    pixel = inter_check(data, angle, &h_y, &y_step, 1);
    h_x = data->cub->player.x + (h_y - data->cub->player.y) / tan(angle);


    if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y') && x_step < 0))
        x_step *= -1;

    while (wall_hit(h_x, h_y - pixel, data))
    {
        h_x += x_step;
        h_y += y_step;
    }

    return sqrt(pow(h_x - data->cub->player.x, 2) + pow(h_y - data->cub->player.y, 2));
}

float get_v_inter(t_data *data, float angle)
{
    float v_x, v_y, x_step, y_step;
    int pixel;

    x_step = TILE_SIZE; 
    y_step = TILE_SIZE * tan(angle);
    v_x = floor(data->cub->player.x / TILE_SIZE) *TILE_SIZE;
    pixel = inter_check(data, angle, &v_x, &x_step, 0);
    v_y = data->cub->player.y + (v_x - data->cub->player.x) * tan(angle);

    if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x') && y_step > 0))
        y_step *= -1;

    while (wall_hit(v_x - pixel, v_y, data))
    {
        v_x += x_step;
        v_y += y_step;
    }

    return sqrt(pow(v_x - data->cub->player.x, 2) + pow(v_y - data->cub->player.y, 2));
}

void castRay(t_data *data, float rayAngle, int stripId)
{
    float h_inter, v_inter;
    rayAngle = normalizeAngle(rayAngle);

    h_inter = get_h_inter(data, rayAngle);
    v_inter = get_v_inter(data, rayAngle);

    if (v_inter <= h_inter)
    {
        if (data->rays->rayAngle > FOV_ANGLE / 2)
            data->rays->c = 'W';
        else
            data->rays->c = 'N';
        data->rays[stripId].distance = v_inter;
        data->rays[stripId].wasHitVertical = TRUE;
    }
    else
    {
        data->rays[stripId].distance = h_inter;
        data->rays[stripId].wasHitVertical = FALSE;
    }

    data->rays[stripId].rayAngle = rayAngle;
}

int my_mlx_pixel_get(int flage, t_data *data, double wallX, int y, int wallHeight) 
{
    t_dir_texture texter[4];
    
    if (!data || flage < 0 || flage > 3)
        return 0;
        
    texter[0] = data->cub->textures.ea;
    texter[1] = data->cub->textures.no;
    texter[2] = data->cub->textures.so;
    texter[3] = data->cub->textures.we;

    int tex_x = (int)(wallX * texter[flage].texter_with);
    int tex_y = (int)((double)y * texter[flage].texter_height / wallHeight);

    tex_x = tex_x < 0 ? 0 : tex_x;
    tex_y = tex_y < 0 ? 0 : tex_y;
    tex_x = tex_x >= texter[flage].texter_with ? texter[flage].texter_with - 1 : tex_x;
    tex_y = tex_y >= texter[flage].texter_height ? texter[flage].texter_height - 1 : tex_y;

    if (!texter[flage].image_pixel_ptr || 
        texter[flage].bits_per_pixel < 8 || 
        texter[flage].line_len <= 0)
    {
        return 0;
    }

    char *pixel_ptr = texter[flage].image_pixel_ptr + 
                     (tex_y * texter[flage].line_len + 
                      tex_x * (texter[flage].bits_per_pixel / 8));

    return *(unsigned int*)pixel_ptr;
}




void drawing_east(t_data *data){
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT / 2; y++) {
            my_pixel_put(&data->img, x, y, create_trgb(0, 100, 100, 100));
        }
    }
}

void drawing_floor(t_data *data){
        for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y++) {
            my_pixel_put(&data->img, x, y, create_trgb(0, 50, 50, 50));
        }
    }
}


void render_walls(t_data *data) {
    if (!data || !data->cub )
        return;
    int texture_id;
    int wallHeight;
    int wallTop;
    int wallBottom;
    int texColor;
    float darkness;

    for (int i = 0; i < NUM_RAYS; i++) {
        float perpDistance = data->rays[i].distance * 
                           cos(data->rays[i].rayAngle - data->cub->player.rotationAngle);
        
        wallHeight = (int)((WINDOW_HEIGHT / perpDistance) * TILE_SIZE);
        
        wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        
        wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        if (wallBottom >= WINDOW_HEIGHT) wallBottom = WINDOW_HEIGHT - 1;

        double rayAngle = normalizeAngle(data->rays[i].rayAngle);
        
        if (data->rays[i].wasHitVertical) {
            if (cos(rayAngle) > 0) {
                texture_id = 3; 
            } else {
                texture_id = 2; 
            }
        } else {
            if (sin(rayAngle) > 0) {
                texture_id = 0;
            } else {
                texture_id = 1; 
            }
        }

        double wallX;
        if (data->rays[i].wasHitVertical) {
            wallX = fmod(data->cub->player.y + data->rays[i].distance * 
                        sin(data->rays[i].rayAngle),TILE_SIZE) / TILE_SIZE;
        } else {
            wallX = fmod(data->cub->player.x + data->rays[i].distance * 
                        cos(data->rays[i].rayAngle), TILE_SIZE) / TILE_SIZE;
        }

        for (int y = wallTop; y < wallBottom; y++) {
            texColor = my_mlx_pixel_get(texture_id, data, wallX, y - wallTop, wallHeight);
            
            darkness = 1.0f + (perpDistance / (TILE_SIZE* 5));
            
            int r = ((texColor >> 16) & 0xFF) / darkness;
            int g = ((texColor >> 8) & 0xFF) / darkness;
            int b = (texColor & 0xFF) / darkness;
            
            r = (r < 0) ? 0 : (r > 255) ? 255 : r;
            g = (g < 0) ? 0 : (g > 255) ? 255 : g;
            b = (b < 0) ? 0 : (b > 255) ? 255 : b;

            int finalColor = create_trgb(0, r, g, b);
            my_pixel_put(&data->img, i, y, finalColor);
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}
void castAllRays(t_data *data) {
    float rayAngle = data->cub->player.rotationAngle - (FOV_ANGLE / 2);
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        castRay(data, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

int update_player(t_player *player, t_map *map)
{
    if (!player || !map)
        return -1;

    double moveStep;
    double newPlayerX;
    double newPlayerY;
    player->rotationAngle = normalizeAngle(player->rotationAngle + 
                           player->turnDirection * player->rotationSpeed);

    moveStep = player->walkDirection * player->moveSpeed;
    newPlayerX = player->x + cos(player->rotationAngle) * moveStep;
    newPlayerY = player->y + sin(player->rotationAngle) * moveStep;

    if (!has_wall_at(map, newPlayerX, newPlayerY))
    {
        player->x = newPlayerX;
        player->y = newPlayerY;
    }
    return 0;
}

int key_press(int keycode, t_data *data)
{
    if (!data )
        return -1;

    if (keycode == KEY_UP)
        data->cub->player.walkDirection = 1;
    else if (keycode == KEY_DOWN)
        data->cub->player.walkDirection = -1;
    else if (keycode == KEY_LEFT)
        data->cub->player.turnDirection = -1;
    else if (keycode == KEY_RIGHT)
        data->cub->player.turnDirection = 1;
    else if (keycode == KEY_ESC)
        close_window(data);
    return 0;
}

int key_release(int keycode, t_data *data)
{
    if (!data)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_DOWN)
       data->cub->player.walkDirection = 0;
    else if (keycode == KEY_LEFT || keycode == KEY_RIGHT)
        data->cub->player.turnDirection = 0;
    return 0;
}

// int read_map(t_data *data, char *map_path)
// {
//     int fd = open(map_path, O_RDONLY);
//     if (fd == -1)
//         return 0;

//     char *line;
//     char *content = NULL;
//     char *temp;

//     while ((line = get_next_line(fd)))
//     {
//         temp = ft_strjoin(content, line);
//         free(line);
//         content = temp;
//         if (!content)
//             break;
//     }

//     if (!content)
//     {
//         close(fd);
//         return 0;
//     }

//     data->map->grid = ft_split(content, '\n');
//     free(content);
//     close(fd);

//     if (!data->map->grid)
//         return 0;

//     get_map_resolution(data->map);
//     data->map->tile_size = TILE_SIZE;
//     return 1;
// }

int game_loop(t_data *data) {
    update_player(&data->cub->player, &data->cub->map);
    mlx_clear_window(data->mlx, data->win);
    castAllRays(data);
    memset(data->img.image_pixel_ptr, 0,  WINDOW_WIDTH * WINDOW_HEIGHT * (data->img.bits_per_pixel / 8));
    drawing_east(data);
    drawing_floor(data);
    render_walls(data);
    return 0;
}

void init_player(t_data *data){

    data->cub->player.radius = 3;
    data->cub->player.turnDirection = 0;
    data->cub->player.walkDirection = 0;
    data->cub->player.rotationAngle = M_PI / 2;
    data->cub->player.moveSpeed = 5;
    data->cub->player.rotationSpeed = 2 * (M_PI / 180);

    // if (has_wall_at(data->map, data->player->x, data->player->y))
    // {
    //     data->player->x = startX - data->map->tile_size;
    //     data->player->y = startY - data->map->tile_size;
    // }

}

bool init_textures(t_data *data){
    if (!data || !data->cub || !data->mlx)
        return false;

    t_texture *texters = &data->cub->textures;
    texters->ea.img_ptr = mlx_xpm_file_to_image(data->mlx,texters->ea.path, &texters->ea.texter_with,&texters->ea.texter_height);

    if (!texters->ea.img_ptr)
        return false;
    texters->ea.image_pixel_ptr = mlx_get_data_addr(texters->ea.img_ptr, 
                                              &texters->ea.bits_per_pixel,
                                              &texters->ea.line_len,
                                             &texters->ea.endian);

    if (!texters->ea.image_pixel_ptr)
        return false;
    texters->no.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->no.path, &texters->no.texter_with, &texters->no.texter_height);
    if (!texters->no.img_ptr)
        return false;
    texters->no.image_pixel_ptr = mlx_get_data_addr(texters->no.img_ptr, 
                                              &texters->no.bits_per_pixel,
                                              &texters->no.line_len,
                                              &texters->no.endian);
     if (!texters->no.image_pixel_ptr)
        return false;
    texters->so.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->so.path, &texters->so.texter_with, &texters->so.texter_height);
    if (!texters->so.img_ptr)
        return false;
    texters->so.image_pixel_ptr = mlx_get_data_addr(texters->so.img_ptr, 
                                              &texters->so.bits_per_pixel,
                                              &texters->so.line_len,
                                               &texters->so.endian);
    if (!texters->so.image_pixel_ptr)
        return false;
    texters->we.img_ptr = mlx_xpm_file_to_image(data->mlx, texters->we.path, &texters->we.texter_with, &texters->we.texter_height);
    if (!texters->we.img_ptr)
        return false;
    texters->we.image_pixel_ptr = mlx_get_data_addr(texters->we.img_ptr, 
                                              &texters->we.bits_per_pixel,
                                              &texters->we.line_len,
                                              &texters->we.endian);
    if (!texters->we.image_pixel_ptr)
        return false;
    return(true);
}

void init_game(t_data *data)
{
    if (!data)
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
}


bool init(t_data *data, char *argv)
{
    if (!data || !argv)
        return false;

    memset(data, 0, sizeof(t_data));
    
    data->cub = malloc(sizeof(t_cub3d));
    if (!data->cub)
        return false;
    
    load_cub3d_file(argv, data->cub);

    init_game(data);
    if (!data->mlx || !data->win)
    {
        free(data->cub);
        if (data->mlx)
        {
            if (data->win)
                mlx_destroy_window(data->mlx, data->win);
            // mlx_destroy_display(data->mlx);
            free(data->mlx);
        }
        return false;
    }

    init_player(data);
    if (!init_textures(data))
    {
        close_window(data);
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;

    memset(&data, 0, sizeof(t_data));
    // memset(&map, 0, sizeof(t_map));


    if (!init(&data, argv[1]))
        return 1;


    mlx_loop_hook(data.mlx, game_loop, &data);
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_hook(data.win, 17, 0, close_window, &data);
    mlx_loop(data.mlx);

    mlx_destroy_image(data.mlx, data.img.img_ptr);
    mlx_destroy_window(data.mlx, data.win);

    return 0;
}
