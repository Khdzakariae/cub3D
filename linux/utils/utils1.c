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

int create_trgb(int t, int r, int g, int b) {
    return (t << 24) | (r << 16) | (g << 8) | b;
}

void render_walls(t_data *data) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float distance = data->rays[i].distance;  
        int wallHeight = (int)(WINDOW_HEIGHT / distance);  
        int wallTop = (WINDOW_HEIGHT / 2) - (wallHeight / 2);  
        int wallBottom = (WINDOW_HEIGHT / 2) + (wallHeight / 2);  


        float darknessFactor = 1.0f + (distance / 4.0f);

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

raycast() {
    const rays = 200;
    const screenWidth = 800; 
    const sliceWidth = screenWidth / rays;
    const angleStep = this.player.fov / rays;
    const ditherPatternSize = 8;  

    this.ctx.fillStyle = 'rgb(20, 0, 20)';
    this.ctx.fillRect(0, 0, 800, 300);  

    // Ground
    this.ctx.fillStyle = 'rgb(60, 0, 60)';
    this.ctx.fillRect(0, 300, 800, 300); 

    // Walls
    for (let i = 0; i < rays; i++) {
        const rayAngle = this.player.angle - (this.player.fov / 2) + i * angleStep;
        const { distance, wallHeight } = this.castRay(rayAngle);
        this.drawWallSlice(i, distance, wallHeight, ditherPatternSize, sliceWidth);
    }



void  castRay(double angle, t_mlx *mlx)
{
    double rayX = mlx->player.x;
    double rayY = mlx->player.y;
    double dist = 0.0;
    double stepSize = 0.1;
    t_rayResult result;

    // Loop to simulate raymarching (check for intersections with walls)
    while (dist < 1000) { // Max distance for ray casting
        rayX += cos(angle) * stepSize;
        rayY += sin(angle) * stepSize;
        dist += stepSize;

        // Check if the ray hits a wall (for simplicity, just a boundary check)
        if (/* condition to check if we hit a wall */) {
            result.distance = dist;
            result.wallHeight = (int)(HEIGHT / dist); // Height of the wall
            return result;
        }
    }

    result.distance = dist;
    result.wallHeight = 0;
    return result;
}

void drawWallSlice(t_mlx *mlx, int x, int distance, int wallHeight)
{
    int yStart = (HEIGHT - wallHeight) / 2;
    int yEnd = yStart + wallHeight;
    int color = 0x00FF00; // Green for walls (change color based on distance)

    for (int y = yStart; y < yEnd; y++) {
        if (y >= 0 && y < HEIGHT) {
            my_mlx_pixel_put(mlx, x, y, color);
        }
    }
}

void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color)
{
    int pixel_pos = (y * mlx->size_line) + (x * (mlx->bpp / 8));
    *(unsigned int *)(mlx->data + pixel_pos) = color;
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