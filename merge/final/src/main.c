#include "../Includes/cub3d.h"

double normalizeAngle(double angle)
{
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
        angle += 2 * M_PI;
    return angle;
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
    if (!map || !map->map || x < 0 || y < 0)
        return 1;
        
    double margin = 20;
    
    double check_points[4][2] = {
        {x - margin, y - margin}, 
        {x + margin, y - margin},  
        {x - margin, y + margin}, 
        {x + margin, y + margin}   
    };
    
    for (int i = 0; i < 4; i++)
    {
        int mapX = (int)(check_points[i][0] / map->title_size);
        int mapY = (int)(check_points[i][1] / map->title_size);
        
        if (mapY >= map->height || mapX >= map->width)
            return 1;
            
        if (map->map[mapY][mapX] == '1')
            return 1;
    }
    
    return 0;
}

int unit_circle(float angle, char c)
{
    if (c == 'x')
        return (angle > 0 && angle < M_PI);
    else if (c == 'y')
        return (angle > (M_PI / 2) && angle < (3 * M_PI) / 2);
    return 0;
}

int inter_check(t_data *data, float angle, float *inter, float *step, int is_horizon)
{
    if (is_horizon)
    {
        if (angle > 0 && angle < M_PI)
        {
            *inter += data->game.map.title_size;
            return (-1);
        }
        *step *= -1;
    }
    else
    {
        if (!(angle > M_PI / 2 && angle < 3 * M_PI / 2)) 
        {
            *inter += data->game.map.title_size;
            return (-1);
        }
        *step *= -1;
    }
    return (1);
}

int wall_hit(float x, float y, t_data *data)
{
    int x_m, y_m;

    if (x < 0 || x >= data->game.map.width * TILE_SIZE || 
        y < 0 || y >= data->game.map.height * TILE_SIZE)
        return 0;

    x_m = floor(x / data->game.map.title_size);
    y_m = floor(y / data->game.map.title_size);

    if (y_m >= data->game.map.height || x_m >= data->game.map.width)
        return 0;

    if (data->game.map.map[y_m] && x_m < (int)ft_strlen(data->game.map.map[y_m]))
        if (data->game.map.map[y_m][x_m] == '1')
            return 0;

    return 1;
}

float get_h_inter(t_data *data, float angle)
{
    float h_x, h_y, x_step, y_step;
    int pixel;

    y_step = data->game.map.title_size;
    x_step = data->game.map.title_size / tan(angle);
    h_y = floor(data->game.player.y / data->game.map.title_size) * data->game.map.title_size;
    pixel = inter_check(data, angle, &h_y, &y_step, 1);
    h_x = data->game.player.x + (h_y - data->game.player.y) / tan(angle);

    if ((unit_circle(angle, 'y') && x_step > 0) || (!unit_circle(angle, 'y') && x_step < 0))
        x_step *= -1;

    while (wall_hit(h_x, h_y - pixel, data))
    {
        h_x += x_step;
        h_y += y_step;
    }

    return sqrt(pow(h_x - data->game.player.x, 2) + pow(h_y - data->game.player.y, 2));
}

float get_v_inter(t_data *data, float angle)
{
    float v_x, v_y, x_step, y_step;
    int pixel;

    x_step = data->game.map.title_size;
    y_step = data->game.map.title_size * tan(angle);
    v_x = floor(data->game.player.x / data->game.map.title_size) * data->game.map.title_size;
    pixel = inter_check(data, angle, &v_x, &x_step, 0);
    v_y = data->game.player.y + (v_x - data->game.player.x) * tan(angle);

    if ((unit_circle(angle, 'x') && y_step < 0) || (!unit_circle(angle, 'x') && y_step > 0))
        y_step *= -1;

    while (wall_hit(v_x - pixel, v_y, data))
    {
        v_x += x_step;
        v_y += y_step;
    }

    return sqrt(pow(v_x - data->game.player.x, 2) + pow(v_y - data->game.player.y, 2));
}

void castRay(t_data *data, float rayAngle, int stripId)
{
    float h_inter, v_inter;
    rayAngle = normalizeAngle(rayAngle);

    h_inter = get_h_inter(data, rayAngle);
    v_inter = get_v_inter(data, rayAngle);

    if (v_inter <= h_inter)
    {
        data->rays[stripId].distance = v_inter;
        data->rays[stripId].wasHitVertical = TRUE;
        data->rays[stripId].rayAngle = rayAngle;
    }
    else
    {
        data->rays[stripId].distance = h_inter;
        data->rays[stripId].wasHitVertical = FALSE;
        data->rays[stripId].rayAngle = rayAngle;
    }
}

int my_mlx_pixel_get(int flag, t_data *data, double wallX, int y, int wallHeight) 
{
    t_dir_texture texter[4];
    
    if (!data || flag < 0 || flag > 3)
        return 0;
        
    texter[0] = data->game.textures.ea;
    texter[1] = data->game.textures.no;
    texter[2] = data->game.textures.so;
    texter[3] = data->game.textures.we;

    int tex_x = (int)(wallX * texter[flag].texter_with);
    int tex_y = (int)((double)y * texter[flag].texter_height / wallHeight);

    tex_x = tex_x < 0 ? 0 : tex_x;
    tex_y = tex_y < 0 ? 0 : tex_y;
    tex_x = tex_x >= texter[flag].texter_with ? texter[flag].texter_with - 1 : tex_x;
    tex_y = tex_y >= texter[flag].texter_height ? texter[flag].texter_height - 1 : tex_y;

    // if (!texter[flag].image_pixel_ptr ||   )
    //     // texter[flag].line_len <= 0)
    //     return 0;

    char *pixel_ptr = texter[flag].image_pixel_ptr + 
                     (tex_y * texter[flag].line_len + 
                      tex_x * (texter[flag].bits_per_pixel / 8));

    return *(unsigned int*)pixel_ptr;
}

void drawing_east(t_data *data)
{
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        for (int y = 0; y < WINDOW_HEIGHT / 2; y++)
        {
            my_pixel_put(&data->img, x, y, data->game.colors.ceiling.color);
        }
    }
}

void drawing_floor(t_data *data)
{
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        for (int y = WINDOW_HEIGHT / 2; y < WINDOW_HEIGHT; y++)
        {
            my_pixel_put(&data->img, x, y, data->game.colors.floor.color);
        }
    }
}

void render_walls(t_data *data)
{
    int texture_id;
    int wallHeight;
    int wallTop;
    int wallBottom;
    int texColor;
    float darkness;

    for (int i = 0; i < NUM_RAYS; i++)
    {
        float perpDistance = data->rays[i].distance * 
                           cos(data->rays[i].rayAngle - data->game.player.rotationAngle);
        
        wallHeight = (int)((WINDOW_HEIGHT / perpDistance) * data->game.map.title_size);
        wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        wallTop = wallTop ;
        
        wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        wallBottom = wallBottom >= WINDOW_HEIGHT ? WINDOW_HEIGHT - 1 : wallBottom;

        double rayAngle = normalizeAngle(data->rays[i].rayAngle);
        
        if (data->rays[i].wasHitVertical)
            texture_id = cos(rayAngle) > 0 ? 3 : 2;
        else
            texture_id = sin(rayAngle) > 0 ? 0 : 1;

        double wallX;
        if (data->rays[i].wasHitVertical)
        {
            wallX = fmod(data->game.player.y + data->rays[i].distance * 
                        sin(data->rays[i].rayAngle), data->game.map.title_size) / data->game.map.title_size;
        }
        else
        {
            wallX = fmod(data->game.player.x + data->rays[i].distance * 
                        cos(data->rays[i].rayAngle), data->game.map.title_size) / data->game.map.title_size;
        }

        for (int y = wallTop; y < wallBottom; y++)
        {
            texColor = my_mlx_pixel_get(texture_id, data, wallX, y - wallTop, wallHeight);
            
            darkness = 1.0f + (perpDistance / (data->game.map.title_size * 5));
            
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

void castAllRays(t_data *data)
{
    float rayAngle = data->game.player.rotationAngle - (FOV_ANGLE / 2);
    
    for (int stripId = 0; stripId < NUM_RAYS; stripId++)
    {
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
    if (!data)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_W)
        data->game.player.walkDirection = 1;
    else if (keycode == KEY_DOWN || keycode == KEY_S)
        data->game.player.walkDirection = -1;
    else if (keycode == KEY_LEFT || keycode == KEY_A)
        data->game.player.turnDirection = -1;
    else if (keycode == KEY_RIGHT || keycode == KEY_D)
        data->game.player.turnDirection = 1;
    else if (keycode == KEY_ESC)
        exit(0);
        
    return 0;
}

int key_release(int keycode, t_data *data)
{
    if (!data)
        return -1;

    if (keycode == KEY_UP || keycode == KEY_DOWN || 
        keycode == KEY_W || keycode == KEY_S)
        data->game.player.walkDirection = 0;
    else if (keycode == KEY_LEFT || keycode == KEY_RIGHT || 
             keycode == KEY_A || keycode == KEY_D)
        data->game.player.turnDirection = 0;
    return 0;
}

int game_loop(t_data *data)
{
    update_player(&data->game.player, &data->game.map);
    mlx_clear_window(data->mlx, data->win);
    
    ft_memset(data->img.image_pixel_ptr, 0, 
             WINDOW_WIDTH * WINDOW_HEIGHT * (data->img.bits_per_pixel / 8));
             
    drawing_east(data);
    drawing_floor(data);
    castAllRays(data);
    render_walls(data);
    return 0;
}

void init_player(t_data *data)
{
    data->game.player.radius = 3;
    data->game.player.turnDirection = 0;
    data->game.player.walkDirection = 0;
    data->game.player.rotationAngle = M_PI / 2;
    data->game.player.moveSpeed = 4.0;  
    data->game.player.rotationSpeed = 3 * (M_PI / 180); 

    if (data->game.player.x == 0 && data->game.player.y == 0)
    {
        data->game.player.x = data->game.map.title_size * 1.5;
        data->game.player.y = data->game.map.title_size * 1.5;
    }
}

void init_textures(t_data *data)
{
    t_texture *texters = &data->game.textures;

    texters->ea.img_ptr = mlx_xpm_file_to_image(data->mlx, 
                                               texters->ea.path, 
                                               &texters->ea.texter_with,
                                               &texters->ea.texter_height);
    if (texters->ea.img_ptr)
        texters->ea.image_pixel_ptr = mlx_get_data_addr(texters->ea.img_ptr, 
                                                   &texters->ea.bits_per_pixel,
                                                   &texters->ea.line_len,
                                                   &texters->ea.endian);

    texters->no.img_ptr = mlx_xpm_file_to_image(data->mlx, 
                                               texters->no.path, 
                                               &texters->no.texter_with,
                                               &texters->no.texter_height);
    if (texters->no.img_ptr)
        texters->no.image_pixel_ptr = mlx_get_data_addr(texters->no.img_ptr, 
                                                   &texters->no.bits_per_pixel,
                                                   &texters->no.line_len,
                                                   &texters->no.endian);

    texters->so.img_ptr = mlx_xpm_file_to_image(data->mlx, 
                                               texters->so.path, 
                                               &texters->so.texter_with,
                                               &texters->so.texter_height);
    if (texters->so.img_ptr)
        texters->so.image_pixel_ptr = mlx_get_data_addr(texters->so.img_ptr, 
                                                   &texters->so.bits_per_pixel,
                                                   &texters->so.line_len,
                                                   &texters->so.endian);

    texters->we.img_ptr = mlx_xpm_file_to_image(data->mlx, 
                                               texters->we.path, 
                                               &texters->we.texter_with,
                                               &texters->we.texter_height);
    if (texters->we.img_ptr)
        texters->we.image_pixel_ptr = mlx_get_data_addr(texters->we.img_ptr, 
                                                   &texters->we.bits_per_pixel,
                                                   &texters->we.line_len,
                                                   &texters->we.endian);
}

void init_game(t_data *data)
{
    if (!data)
        return;

    data->mlx = mlx_init();
    if (!data->mlx)
        return;

    data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    if (!data->win)
    {
        // mlx_destroy_display(data->mlx);
        free(data->mlx);
        return;
    }

    // Create image buffer
    data->img.img_ptr = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!data->img.img_ptr)
    {
        // mlx_destroy_window(data->mlx, data->win);
        // mlx_destroy_display(data->mlx);
        free(data->mlx);
        return;
    }

    data->img.image_pixel_ptr = mlx_get_data_addr(data->img.img_ptr, 
                                             &data->img.bits_per_pixel,
                                             &data->img.line_len,
                                             &data->img.endian);
}

bool init(t_data *data)
{
    init_game(data);
    if (!data->mlx || !data->win || !data->img.img_ptr)
        return false;

    init_player(data);
    init_textures(data);

    return true;
}

int cleanup(t_data *data)
{

    t_texture *texters = &data->game.textures;
    if (texters->ea.img_ptr)
        mlx_destroy_image(data->mlx, texters->ea.img_ptr);
    if (texters->no.img_ptr)
        mlx_destroy_image(data->mlx, texters->no.img_ptr);
    if (texters->so.img_ptr)
        mlx_destroy_image(data->mlx, texters->so.img_ptr);
    if (texters->we.img_ptr)
        mlx_destroy_image(data->mlx, texters->we.img_ptr);

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
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;
    ft_memset(&data, 0, sizeof(t_data));

    t_cub3d cub3d;
    load_cub3d_file(argv[1], &cub3d);
    data.game = cub3d;

    if (!init(&data))
    {
        fprintf(stderr, "Error: Failed to initialize game\n");
        cleanup(&data);
        return 1;
    }

    mlx_loop_hook(data.mlx, game_loop, &data);
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_hook(data.win, 17, 0, cleanup, &data);

    mlx_loop(data.mlx);

    return 0;
}