#include "../Includes/cub3d.h"

// #include <mlx.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define TILE_SIZE 32
#define MAP_NUM_ROWS 11
#define MAP_NUM_COLS 15
#define WINDOW_WIDTH (MAP_NUM_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)

#define KEY_UP 126
#define KEY_DOWN 125
#define KEY_LEFT 123
#define KEY_RIGHT 124
#define KEY_ESC 53

typedef struct s_map {
	int		map_height;
	int 	map_width;
    char **grid;
} t_map;

typedef struct s_player {
    double x;
    double y;
    double radius;
    double turnDirection;
    double walkDirection;
    double rotationAngle;
    double moveSpeed;
    double rotationSpeed;
} t_player;

typedef struct s_data {
    void *mlx;
    void *win;
    t_map *map;
    t_player *player;
} t_data;


void	get_map_resolution(t_map *map)
{
	map->map_height = 0;
	map->map_width = 0;
	for (int i = 0; map->grid[i] != NULL; i++)
	{
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
    int i, j;
    for (i = 0; i < TILE_SIZE; i++) {
        for (j = 0; j < TILE_SIZE; j++) {
            mlx_pixel_put(data->mlx, data->win, x + j, y + i, color);
        }
    }
}


int has_wall_at(t_map *map, double x, double y)
{
    if (x < 0 || x >= map->map_width * TILE_SIZE || y < 0 || y >= map->map_height * TILE_SIZE )
        return 1;
    int mapX = (int)(x / TILE_SIZE);
    int mapY = (int)(y / TILE_SIZE);
    if (map->grid[mapY][mapX] == '1')
        return 1;
    return 0;
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
    int i, j;
    for (i = 0; i < data->map->map_width; i++) {
        for (j = 0; j < data->map->map_height; j++) {
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
    int i, j;

    for (i = -radius; i < radius; i++) {
        for (j = -radius; j < radius; j++) {
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
    render_map(data);
    render_player(data);
    return 0;
}

int	read_map(t_data *data, char *map)
{
	int		k;
	char	*s;
	char	*ret;

	ret = NULL;
	k = open(map, O_RDONLY);
	if (k == -1)
		return (write(1, "ENTRER MAP VALID !", 24), 0);
	s = get_next_line(k);
	if (!s)
		return (write(1,  "FILLE VIDE !!", 20), 0);
	while (s)
	{
		ret = ft_strjoin(ret, s);
		free(s);
		s = get_next_line(k);
	}
	data->map->grid = ft_split(ret, 10);
	free(ret);
	get_map_resolution(data->map);
	return (close(k), 1);
}

int main(int ac, char **av) {
	if (ac > 1){
		t_data data;
		t_player player;

		if (read_map(&data, av[1]) == 0)
			return (0);
		data.mlx = mlx_init();
		data.win = mlx_new_window(data.mlx, (data.map->map_width * TILE_SIZE ), (data.map->map_height * TILE_SIZE), "2D Grid Map");


		data.player = &player;

		player.x = data.map->map_width * TILE_SIZE  / 2;
		player.y = data.map->map_height * TILE_SIZE / 2;
		player.radius = 3;
		player.turnDirection = 0;
		player.walkDirection = 0;
		player.rotationAngle = M_PI / 2;
		player.moveSpeed = 10.0;
		player.rotationSpeed = 10.0 * (M_PI / 180);
		mlx_hook(data.win, 2, 1L << 0, key_press, &data);
		mlx_hook(data.win, 3, 1L << 1, key_release, &data);
		mlx_loop_hook(data.mlx, game_loop, &data);
		mlx_loop(data.mlx);
	}
    return 0;
}