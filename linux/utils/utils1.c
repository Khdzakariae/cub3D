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