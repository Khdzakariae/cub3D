#include "../Includes/cub3d.h"

// void render_walls(t_data *data) {
//     for (int i = 0; i < NUM_RAYS; i++) {
//          float distance = data->rays[i].distance;  
//         int wallHeight = (int)(WINDOW_HEIGHT / distance);  
//         int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);  
//         int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);  


//         float darknessFactor = 1.0f + (distance / 16.0f);

//         int dither = data->rays[i].wallHitContent;
//         int baseColor = 200 + dither;
//         int adjustedColor = (int)(baseColor / darknessFactor);
//         if (adjustedColor < 0) adjustedColor = 0;
//         if (adjustedColor > 255) adjustedColor = 255;
//         int wallColor = create_trgb(0, adjustedColor, 0, adjustedColor);

//         for (int y = wallTop; y < wallBottom; y++) {
//             my_pixel_put(&data->img, i , y, wallColor);
//         }
// }
//     mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
// }

// void castAllRays(t_data *data) {
//     float rayAngle = data->player->rotationAngle - (FOV_ANGLE / 2);
//     for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
//         castRay(data, rayAngle, stripId);
//         rayAngle += FOV_ANGLE / NUM_RAYS;
//     }
// }
void my_pixel_put(t_img *img, int x, int y, int color) {
    if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
    {
        int offset = (y * img->line_len) + (x * (img->bits_per_pixel / 8));
        *((unsigned int *)(img->image_pixel_ptr + offset)) = color;

    }
}

int create_trgb(int t, int r, int g, int b) {
    return (t << 24) | (r << 16) | (g << 4) | b;
}
int create_trgb1(int t, int r, int g, int b) {
    return (t << 16) | (r << 26) | (g << 4) | b;
}
void render_ard(t_data *data) {
    for (int i = 0; i < NUM_RAYS; i++) {
        int baseColor = 150 - i / 8;
        int adjustedColor = (int)(baseColor );
        int ardColor = create_trgb1(0, adjustedColor, 0, adjustedColor);
        for (int y = 0; y < WINDOW_HEIGHT / 2; y++) {
            my_pixel_put(&data->img, i , y, ardColor);
        }
    }
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
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
        mlx_clear_window(data->mlx, data->win);
        mlx_destroy_window(data->mlx, data->win);
        exit (0);
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
double normalizeAngle(double angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
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
    int horzWallContent = 0;

    yintercept = floor(data->player->y / TILE_SIZE) * TILE_SIZE;
    yintercept += isRayFacingDown ? TILE_SIZE : 0;
    xintercept = data->player->x + (yintercept - data->player->y) / tan(rayAngle);
    ystep = TILE_SIZE;
    ystep *= isRayFacingUp ? -1 : 1;
    xstep = TILE_SIZE / tan(rayAngle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
        if (has_wall_at(data->map, nextHorzTouchX, nextHorzTouchY)) {
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = data->map->grid[(int)(horzWallHitY / TILE_SIZE)][(int)(horzWallHitX / TILE_SIZE)];
            foundHorzWallHit = TRUE;
            break;
        }
        nextHorzTouchX += xstep;
        nextHorzTouchY += ystep;
    }

    int foundVertWallHit = FALSE;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallContent = 0;

    xintercept = floor(data->player->x / TILE_SIZE) * TILE_SIZE;
    xintercept += isRayFacingRight ? TILE_SIZE : 0;
    yintercept = data->player->y + (xintercept - data->player->x) * tan(rayAngle);
    xstep = TILE_SIZE;
    xstep *= isRayFacingLeft ? -1 : 1;
    ystep = TILE_SIZE * tan(rayAngle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

    while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
        if (has_wall_at(data->map, nextVertTouchX, nextVertTouchY)) {
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallContent = data->map->grid[(int)(vertWallHitY / TILE_SIZE)][(int)(vertWallHitX / TILE_SIZE)];
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
        data->rays[stripId].wallHitContent = horzWallContent;
        data->rays[stripId].wasHitVertical = FALSE;
    } else {
        data->rays[stripId].distance = vertDistance;
        data->rays[stripId].wallHitX = vertWallHitX;
        data->rays[stripId].wallHitY = vertWallHitY;
        data->rays[stripId].wallHitContent = vertWallContent;
        data->rays[stripId].wasHitVertical = TRUE;
    }
}


void render_walls(t_data *data) {
    int i = 0;
    while (i < NUM_RAYS) {
        castRay(data, data->player->rotationAngle + (FOV_ANGLE / 2) - (i * (FOV_ANGLE / NUM_RAYS)), i);
        float distance = data->rays[i].distance;  
        int wallHeight = (int)(WINDOW_HEIGHT / distance);  
        int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);  
        int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);  


        float darknessFactor = 1.0f + (distance / 16.0f);

        int dither = data->rays[i].wallHitContent;
        int baseColor = 200 + dither;
        int adjustedColor = (int)(baseColor / darknessFactor);
        if (adjustedColor < 0) adjustedColor = 0;
        if (adjustedColor > 255) adjustedColor = 255;
        int wallColor = create_trgb(0, adjustedColor, 0, adjustedColor);

        for (int y = wallTop; y < wallBottom; y++) {
            my_pixel_put(&data->img, i , y, wallColor);
        }
        i++;
    }
}

int game_loop(t_data *data) {
    if (data->img.img_ptr == NULL) {
        data->img.img_ptr = mlx_new_image(data->mlx , WINDOW_WIDTH, WINDOW_HEIGHT);
        data->img.image_pixel_ptr = mlx_get_data_addr(data->img.img_ptr, &data->img.bits_per_pixel, &data->img.line_len, &data->img.endien);
    }
    
    update_player(data->player, data->map);
    mlx_clear_window(data->mlx, data->win);
    render_ard(data); 
    render_walls(data);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img_ptr, 0, 0);
    
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
    player->x = 300;
    player->y = 300;
    player->radius = 3;
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->rotationAngle = M_PI / 2;
    player->moveSpeed = 0.03;
    player->rotationSpeed = 0.9 * (M_PI / 180);
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
    // exit(0);

    init_game(&data, &player);
    mlx_loop_hook(data.mlx, game_loop, &data); 
    mlx_hook(data.win, 2, 1L<<0, key_press, &data);  
    mlx_hook(data.win, 3, 1L<<1, key_release, &data);
    mlx_loop(data.mlx); 
    return 0;
}
