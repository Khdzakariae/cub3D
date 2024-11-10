#include "../Includes/cub3d.h"

// New structure to hold raycasting data

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

double normalizeAngle(double angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
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

void initRayData(t_data *data, float rayAngle, RaycastData *rayData) {
    (void)data;
    rayData->rayAngle = normalizeAngle(rayAngle);
    rayData->isRayFacingDown = rayAngle > 0 && rayAngle < M_PI;
    rayData->isRayFacingUp = !rayData->isRayFacingDown;
    rayData->isRayFacingRight = rayAngle < 0.5 * M_PI || rayAngle > 1.5 * M_PI;
    rayData->isRayFacingLeft = !rayData->isRayFacingRight;
}

void castHorizontalRay(t_data *data, float rayAngle, RaycastData *rayData) {
    rayData->yintercept = floor(data->player->y / TILE_SIZE) * TILE_SIZE;
    rayData->yintercept += rayData->isRayFacingDown ? TILE_SIZE : 0;
    rayData->xintercept = data->player->x + (rayData->yintercept - data->player->y) / tan(rayAngle);
    rayData->ystep = TILE_SIZE * (rayData->isRayFacingUp ? -1 : 1);
    rayData->xstep = TILE_SIZE / tan(rayAngle);
    rayData->xstep *= (rayData->isRayFacingLeft && rayData->xstep > 0) ? -1 : 1;
    rayData->xstep *= (rayData->isRayFacingRight && rayData->xstep < 0) ? -1 : 1;

    float nextHorzTouchX = rayData->xintercept;
    float nextHorzTouchY = rayData->yintercept;
    rayData->foundHorzHit = FALSE;

    while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (rayData->isRayFacingUp ? -1 : 0);
        if (has_wall_at(data->map, xToCheck, yToCheck)) {
            rayData->horzHitX = nextHorzTouchX;
            rayData->horzHitY = nextHorzTouchY;
            rayData->horzWallContent = data->map->grid[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
            rayData->foundHorzHit = TRUE;
            break;
        } else {
            nextHorzTouchX += rayData->xstep;
            nextHorzTouchY += rayData->ystep;
        }
    }
}

void castVerticalRay(t_data *data, float rayAngle, RaycastData *rayData) {
    rayData->xintercept = floor(data->player->x / TILE_SIZE) * TILE_SIZE;
    rayData->xintercept += rayData->isRayFacingRight ? TILE_SIZE : 0;
    rayData->yintercept = data->player->y + (rayData->xintercept - data->player->x) * tan(rayAngle);
    rayData->xstep = TILE_SIZE * (rayData->isRayFacingLeft ? -1 : 1);
    rayData->ystep = TILE_SIZE * tan(rayAngle);
    rayData->ystep *= (rayData->isRayFacingUp && rayData->ystep > 0) ? -1 : 1;
    rayData->ystep *= (rayData->isRayFacingDown && rayData->ystep < 0) ? -1 : 1;

    float nextVertTouchX = rayData->xintercept;
    float nextVertTouchY = rayData->yintercept;
    rayData->foundVertHit = FALSE;

    while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextVertTouchX + (rayData->isRayFacingLeft ? -1 : 0);
        float yToCheck = nextVertTouchY;
        if (has_wall_at(data->map, xToCheck, yToCheck)) {
            rayData->vertHitX = nextVertTouchX;
            rayData->vertHitY = nextVertTouchY;
            rayData->vertWallContent = data->map->grid[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
            rayData->foundVertHit = TRUE;
            break;
        } else {
            nextVertTouchX += rayData->xstep;
            nextVertTouchY += rayData->ystep;
        }
    }
}

void calculateHitDistances(t_data *data, RaycastData *rayData) {
    rayData->horzHitDistance = rayData->foundHorzHit
        ? distanceBetweenPoints(data->player->x, data->player->y, rayData->horzHitX, rayData->horzHitY)
        : INT_MAX;
    rayData->vertHitDistance = rayData->foundVertHit
        ? distanceBetweenPoints(data->player->x, data->player->y, rayData->vertHitX, rayData->vertHitY)
        : INT_MAX;
}

void storeRayResults(int stripId, RaycastData *rayData) {
    if (rayData->vertHitDistance < rayData->horzHitDistance) {
        rays[stripId].distance = rayData->vertHitDistance;
        rays[stripId].wallHitX = rayData->vertHitX;
        rays[stripId].wallHitY = rayData->vertHitY;
        rays[stripId].wallHitContent = rayData->vertWallContent;
        rays[stripId].wasHitVertical = TRUE;
    } else {
        rays[stripId].distance = rayData->horzHitDistance;
        rays[stripId].wallHitX = rayData->horzHitX;
        rays[stripId].wallHitY = rayData->horzHitY;
        rays[stripId].wallHitContent = rayData->horzWallContent;
        rays[stripId].wasHitVertical = FALSE;
    }

    rays[stripId].rayAngle = rayData->rayAngle;
    rays[stripId].isRayFacingDown = rayData->isRayFacingDown;
    rays[stripId].isRayFacingUp = rayData->isRayFacingUp;
    rays[stripId].isRayFacingLeft = rayData->isRayFacingLeft;
    rays[stripId].isRayFacingRight = rayData->isRayFacingRight;
}

void castRay(t_data *data, float rayAngle, int stripId) {
    RaycastData rayData;
    
    initRayData(data, rayAngle, &rayData);
    castHorizontalRay(data, rayAngle, &rayData);
    castVerticalRay(data, rayAngle, &rayData);
    calculateHitDistances(data, &rayData);
    storeRayResults(stripId, &rayData);
}



// void castRay(t_data *data, float rayAngle, int stripId) {
//     rayAngle = normalizeAngle(rayAngle);

//     int isRayFacingDown = rayAngle > 0 && rayAngle < M_PI;
//     int isRayFacingUp = !isRayFacingDown;

//     int isRayFacingRight = rayAngle < 0.5 * M_PI || rayAngle > 1.5 * M_PI;
//     int isRayFacingLeft = !isRayFacingRight;

//     float xintercept, yintercept;
//     float xstep, ystep;

//     int foundHorzWallHit = FALSE;
//     float horzWallHitX = 0;
//     float horzWallHitY = 0;
//     int horzWallContent = 0;
//     yintercept = floor(data->player->y / TILE_SIZE) * TILE_SIZE;
//     yintercept += isRayFacingDown ? TILE_SIZE : 0;
//     xintercept = data->player->x + (yintercept - data->player->y) / tan(rayAngle);
//     ystep = TILE_SIZE;
//     ystep *= isRayFacingUp ? -1 : 1;

//     xstep = TILE_SIZE / tan(rayAngle);
//     xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
//     xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;
//     float nextHorzTouchX = xintercept;
//     float nextHorzTouchY = yintercept;
//     while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= WINDOW_HEIGHT) {
//         float xToCheck = nextHorzTouchX;
//         float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
//         if (has_wall_at(data->map, xToCheck, yToCheck)) {
//             horzWallHitX = nextHorzTouchX;
//             horzWallHitY = nextHorzTouchY;
//             horzWallContent = data->map->grid[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
//             foundHorzWallHit = TRUE;
//             break;
//         } else {
//             nextHorzTouchX += xstep;
//             nextHorzTouchY += ystep;
//         }
//     }
//     int foundVertWallHit = FALSE;
//     float vertWallHitX = 0;
//     float vertWallHitY = 0;
//     int vertWallContent = 0;
//     xintercept = floor(data->player->x / TILE_SIZE) * TILE_SIZE;
//     xintercept += isRayFacingRight ? TILE_SIZE : 0;
//     yintercept = data->player->y + (xintercept - data->player->x) * tan(rayAngle);
//     xstep = TILE_SIZE;
//     xstep *= isRayFacingLeft ? -1 : 1;
//     ystep = TILE_SIZE * tan(rayAngle);
//     ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
//     ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;
//     float nextVertTouchX = xintercept;
//     float nextVertTouchY = yintercept;
//     while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= WINDOW_HEIGHT) {
//         float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
//         float yToCheck = nextVertTouchY;
//         if (has_wall_at(data->map, xToCheck, yToCheck)) {
//             vertWallHitX = nextVertTouchX;
//             vertWallHitY = nextVertTouchY;
//             vertWallContent = data->map->grid[(int)floor(yToCheck / TILE_SIZE)][(int)floor(xToCheck / TILE_SIZE)];
//             foundVertWallHit = TRUE;
//             break;
//         } else {
//             nextVertTouchX += xstep;
//             nextVertTouchY += ystep;
//         }
//     }
//     float horzHitDistance = foundHorzWallHit
//         ? distanceBetweenPoints(data->player->x, data->player->y, horzWallHitX, horzWallHitY)
//         : INT_MAX;
//     float vertHitDistance = foundVertWallHit
//         ? distanceBetweenPoints(data->player->x, data->player->y, vertWallHitX, vertWallHitY)
//         : INT_MAX;
//     if (vertHitDistance < horzHitDistance) {
//         rays[stripId].distance = vertHitDistance;
//         rays[stripId].wallHitX = vertWallHitX;
//         rays[stripId].wallHitY = vertWallHitY;
//         rays[stripId].wallHitContent = vertWallContent;
//         rays[stripId].wasHitVertical = TRUE;
//     } else {
//         rays[stripId].distance = horzHitDistance;
//         rays[stripId].wallHitX = horzWallHitX;
//         rays[stripId].wallHitY = horzWallHitY;
//         rays[stripId].wallHitContent = horzWallContent;
//         rays[stripId].wasHitVertical = FALSE;
//     }
//     rays[stripId].rayAngle = rayAngle;
//     rays[stripId].isRayFacingDown = isRayFacingDown;
//     rays[stripId].isRayFacingUp = isRayFacingUp;
//     rays[stripId].isRayFacingLeft = isRayFacingLeft;
//     rays[stripId].isRayFacingRight = isRayFacingRight;
// }
    // mlx_pixel_put(data->mlx, data->win, (int)rays[stripId].wallHitX, (int)rays[stripId].wallHitY, 0xFFFFFF);

void castAllRays(t_data *data) {
    float rayAngle = data->player->rotationAngle - (FOV_ANGLE / 2);
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        castRay(data, rayAngle, stripId);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
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

void draw_line(t_data *data, int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    while (1) {
        mlx_pixel_put(data->mlx, data->win, x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
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
    printf("Player position updated: x = %.2f, y = %.2f, angle = %.2f\n", player->x, player->y, player->rotationAngle);
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
    castAllRays(data);
    render_walls(data);
    render_player(data);
    return 0;
}

int read_map(t_data *data, char *map) {
    int k = open(map, O_RDONLY);
    if (k == -1) {
        write(1, "ENTRER MAP VALID !", 18);
        return 0;
    }
    char *s = get_next_line(k);
    if (!s) {
        write(1, "FILLE VIDE !!", 12);
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
    player->moveSpeed = 2.0;
    player->rotationSpeed = 3.0 * (M_PI / 180);
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
