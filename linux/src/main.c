#include "../Includes/cub3d.h"

void my_pixel_put(t_img *img, int x, int y, int color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        int offset = (y * img->line_len) + (x * (img->bits_per_pixel / 8));
        *((unsigned int *)(img->image_pixel_ptr + offset)) = color;

    }
}

int create_trgb(int t, int r, int g, int b) {
    return (t << 16) | (r << 8) | (g << 4) | b;
}

void render_walls(t_data *data) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float distance = data->rays[i].distance;  
        int wallHeight = (int)(WINDOW_HEIGHT / distance);  
        int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);  
        int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);  


        float darknessFactor = 1.0f + (distance / 8.0f);

        int dither = data->rays[i].wallHitContent;
        int baseColor = 180 + dither;
        int adjustedColor = (int)(baseColor / darknessFactor);
        if (adjustedColor < 0) adjustedColor = 0;
        if (adjustedColor > 255) adjustedColor = 255;
        int wallColor = create_trgb(0, adjustedColor, 0, adjustedColor);


        for (int y = wallTop; y < wallBottom; y++) {
            my_pixel_put(&data->img, i , y, wallColor);
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
}
// void render_walls(t_data *data) {
//     for (int i = 0; i < NUM_RAYS; i++) {
//         float distance = data->rays[i].distance;  
//         int wallHeight = (int)(WINDOW_HEIGHT / distance);  
//         int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);  
//         int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);  

//         int wallColor = (data->rays[i].wallHitContent == '1') ? COLOR_BLUE : COLOR_GREEN;

//         for (int y = wallTop; y < wallBottom; y++) {
//             my_pixel_put(&data->img, i, y, wallColor);
//         }
//     }
//     mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
// }

double distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int has_wall_at(t_map *map, double x, double y) {
    if (x < 0 || x >= map->map_width * TILE_SIZE || y < 0 || y >= map->map_height * TILE_SIZE)
        return 1;  
    int mapX = (int)(x / TILE_SIZE);
    int mapY = (int)(y / TILE_SIZE);
    if (map->grid[mapY][mapX] == '1') 
        return 1;
    return 0;
}

void get_map_resolution(t_map *map) {
    map->map_height = 0;
    map->map_width = 0;
    for (int i = 0; map->grid[i] != NULL; i++) {
        map->map_height++;
        int row_length = ft_strlen(map->grid[i]);
        if (row_length > map->map_width)
            map->map_width = row_length;
    }
}

int update_player(t_player *player, t_map *map) {
    player->rotationAngle += player->turnDirection * player->rotationSpeed;
    double moveStep = player->walkDirection * player->moveSpeed;

    double newPlayerX = player->x + cos(player->rotationAngle) * moveStep;
    double newPlayerY = player->y + sin(player->rotationAngle) * moveStep;

    if (!has_wall_at(map, newPlayerX, newPlayerY)) {
        player->x = newPlayerX;
        player->y = newPlayerY;
    }
    return 0;
}

int key_press(int keycode, t_data *data) {
    if (keycode == KEY_UP) {
        data->player->walkDirection = 1;
    } else if (keycode == KEY_DOWN) {
        data->player->walkDirection = -1;
    } else if (keycode == KEY_LEFT) {
        data->player->turnDirection = -1;
    } else if (keycode == KEY_RIGHT) {
        data->player->turnDirection = 1;
    } else if (keycode == KEY_ESC) {
        exit(0);
    }
    return 0;
}

int key_release(int keycode, t_data *data) {
    if (keycode == KEY_UP || keycode == KEY_DOWN) {
        data->player->walkDirection = 0;
    } else if (keycode == KEY_LEFT || keycode == KEY_RIGHT) {
        data->player->turnDirection = 0;
    }
    return 0;
}

int read_map(t_data *data, char *map) {
    int k = open(map, O_RDONLY);
    if (k == -1) {
        return 0;
    }
    char *s = get_next_line(k);
    if (!s) {
        return 0;
    }
    char *ret = NULL;
    while (s) {
        ret = ft_strjoin(ret, s);
        free(s);
        s = get_next_line(k);
    }
    data->map->grid = ft_split(ret, 10);
    free(ret);
    get_map_resolution(data->map);
    close(k);
    return 1;
}

void dda_bresenham(t_data *data, float rayAngle, int stripId) {
    double posX = data->player->x, posY = data->player->y;
    double dirX = cos(rayAngle), dirY = sin(rayAngle);

    int mapX = (int)posX, mapY = (int)posY;

    double deltaDistX = fabs(1 / dirX);
    double deltaDistY = fabs(1 / dirY);

    double errorX, errorY;

    int stepX, stepY;

    if (dirX < 0) {
        stepX = -1;
        errorX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        errorX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (dirY < 0) {
        stepY = -1;
        errorY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        errorY = (mapY + 1.0 - posY) * deltaDistY;
    }

    int hit = 0;
    while (hit == 0) {
        if (errorX < errorY) {
            errorX += deltaDistX;
            mapX += stepX;
        } else {
            errorY += deltaDistY;
            mapY += stepY;
        }

        if (has_wall_at(data->map, mapX, mapY)) {
            hit = 1;
        }
    }

    float horzHitDistance = hit ? distanceBetweenPoints(data->player->x, data->player->y, mapX, mapY) : INT_MAX;
    float vertHitDistance = hit ? distanceBetweenPoints(data->player->x, data->player->y, mapX, mapY) : INT_MAX;

    if (vertHitDistance < horzHitDistance) {
        data->rays[stripId].distance = vertHitDistance;
        data->rays[stripId].wallHitContent = '1'; 
    } else {
        data->rays[stripId].distance = horzHitDistance;
        data->rays[stripId].wallHitContent = '1'; 
    }
}

void castAllRays(t_data *data) {
    float rayAngle = data->player->rotationAngle - (FOV_ANGLE / 2);
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        dda_bresenham(data, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

int game_loop(t_data *data) {
    if (data->img.img_ptr == NULL) {
        data->img.img_ptr = mlx_new_image(data->mlx , WINDOW_WIDTH, WINDOW_HEIGHT);
        data->img.image_pixel_ptr = mlx_get_data_addr(data->img.img_ptr, &data->img.bits_per_pixel, &data->img.line_len, &data->img.endien);
    }

    update_player(data->player, data->map); 
    castAllRays(data);
    mlx_clear_window(data->mlx, data->win); 
    render_walls(data); 

    if (data->img.img_ptr) {
        mlx_destroy_image(data->mlx, data->img.img_ptr); 
        data->img.img_ptr = NULL;  
    }

    return 0;
}

void init_game(t_data *data, t_player *player) {
    data->mlx = mlx_init();
    data->win = mlx_new_window(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "2D Grid Map");
    data->player = player;
    player->x = data->map->map_width * TILE_SIZE / 2;
    player->y = data->map->map_height * TILE_SIZE / 2;
    player->radius = 3;
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->rotationAngle = M_PI / 2;
    player->moveSpeed = 0.03;
    player->rotationSpeed = 0.02 * (M_PI / 180);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1;
    }

    t_data data;
    t_player player;
    t_map map;
    data.map = &map;

    if (!read_map(&data, argv[1])) {
        return 1;
    }

    init_game(&data, &player);
    mlx_loop_hook(data.mlx, game_loop, &data); 
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);  
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_loop(data.mlx); 
    return 0;
}
