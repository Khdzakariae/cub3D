#include "../Includes/cub3d.h"

void render_walls(t_data *data) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float distance = rays[i].distance;
        int wallHeight = (int)(WINDOW_HEIGHT / distance);
        int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        int wallColor = (rays[i].wallHitContent == '1') ? 0x999999 : 0x333333;

        for (int y = wallTop; y < wallBottom; y++) {
            mlx_pixel_put(data->mlx, data->win, i, y, wallColor);
        }
    }
}

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

void DDA(t_data *data,int stripId) {
    double posX = data->player->x, posY = data->player->y;
    double dirX = -1, dirY = 0;
    double planeX = data->player->rotationAngle, planeY = data->player->rotationAngle;
    double cameraX = (2 * stripId / (float)NUM_RAYS) - 1; 
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;

    int mapX = (int)posX, mapY = (int)posY;
    double sideDistX, sideDistY;
    double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
    int stepX, stepY;
    int hit = 0;

    if (rayDirX < 0) {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
    } else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }

    if (rayDirY < 0) {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
    } else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }

    while (hit == 0) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
        } 
        else {
            sideDistY += deltaDistY;
            mapY += stepY;
        }
        if (has_wall_at(data->map, mapX, mapY)) {
            printf("wall position updated: x = %d, y = %d\n", mapX, mapY);
            mlx_pixel_put(data->mlx, data->win, mapX, mapY,0xFF0000 );

            hit = 1;
        }
    }
}


void castAllRays(t_data *data) {
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        DDA(data, stripId);
    }
}

void draw_line(t_data *data, int x0, int y0, int x1, int y1, int color)  
{
    int dx, dy, p, x, y;  
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    x = x0;  
    y = y0;  

    int sx = (x1 > x0) ? 1 : -1; 
    int sy = (y1 > y0) ? 1 : -1;
    
    if (dx > dy) {
        p = 2 * dy - dx; 
        while (x != x1) {
            mlx_pixel_put(data->mlx, data->win, x, y, color); 
            if (p >= 0) {
                y += sy; 
                p -= 2 * dx;
            }
            p += 2 * dy;
            x += sx;
        }
    } else {
        p = 2 * dx - dy; 
        while (y != y1) { 
            mlx_pixel_put(data->mlx, data->win, x, y, color);
            if (p >= 0) {
                x += sx;  
                p -= 2 * dy; 
            }
            p += 2 * dx; 
            y += sy;
        }
    }
    mlx_pixel_put(data->mlx, data->win, x, y, color);
}

void put_tile(t_data *data, int x, int y, int color) {
    for (int i = 0; i < TILE_SIZE; i++) {
        for (int j = 0; j < TILE_SIZE; j++) {
            mlx_pixel_put(data->mlx, data->win, x + j, y + i, color);
        }
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
    // printf("Player position updated: x = %.2f, y = %.2f, angle = %.2f\n", player->x, player->y, player->rotationAngle);
    return 0;
}

int render_map(t_data *data) {
    for (int i = 0; i < data->map->map_width; i++) {
        for (int j = 0; j < data->map->map_height; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = (data->map->grid[i][j] == '1') ? 0x222222 : 0xFFFFFF;
            put_tile(data, tileX, tileY, tileColor);
        }
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

int render_player(t_data *data) {
    int px = (int)data->player->x;
    int py = (int)data->player->y;
    int radius = (int)data->player->radius;
    for (int i = -radius; i < radius; i++) {
        for (int j = -radius; j < radius; j++) {
            if (i * i + j * j <= radius * radius) {
                mlx_pixel_put(data->mlx, data->win, px + i, py + j, 0xFF0000);
            }
        }
    }
    int lineLength = 30;
    int lineEndX = px + cos(data->player->rotationAngle) * lineLength;
    int lineEndY = py + sin(data->player->rotationAngle) * lineLength;
    draw_line(data, px, py, lineEndX, lineEndY, 0xFF0000);
    return 0;
}

int game_loop(t_data *data) {
    update_player(data->player, data->map);
    mlx_clear_window(data->mlx, data->win);
    // DDA(data);
    // render_map(data);
    castAllRays(data);
    // render_walls(data);
    render_player(data);
    return 0;
}

int read_map(t_data *data, char *map) {
    int k = open(map, O_RDONLY);
    if (k == -1) {
        // write(1, "ENTRER MAP VALID !", 18);
        return 0;
    }
    char *s = get_next_line(k);
    if (!s) {
        // write(1, "FILLE VIDE !!", 12);
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

void init_game(t_data *data, t_player *player) {
    data->mlx = mlx_init();
    data->win = mlx_new_window(data->mlx, (data->map->map_width * TILE_SIZE), (data->map->map_height * TILE_SIZE), "2D Grid Map");
    data->player = player;
    player->x = data->map->map_width * TILE_SIZE / 2;
    player->y = data->map->map_height * TILE_SIZE / 2;
    player->radius = 3;
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->rotationAngle = M_PI / 2;
    player->moveSpeed = 0.02;
    player->rotationSpeed = 0.05 * (M_PI / 180);
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
