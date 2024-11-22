#ifndef CUB3D_H
#define CUB3D_H

#include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "get_next_line/get_next_line.h"
# include <string.h>

#define NO_FD -1
#define TILE_SIZE 64

typedef enum	e_bool
{
	false,
	true
}	t_bool;

typedef struct s_cub3d t_cub3d;

typedef struct s_player {
    double  x;
    double  y;
    char    player_direction;
} t_player;

typedef struct s_data
{
	/* mlx data */
	t_cub3d	*cub3d;
}			t_data;

typedef struct s_dir_texture
{
	char	*path;
	t_bool	is_set;
} t_dir_texture;

typedef struct s_texture
{
	t_dir_texture	no;
	t_dir_texture	so;
	t_dir_texture	we;
	t_dir_texture	ea;
} t_texture;

typedef struct s_color
{
	int		color;
	//char *color;
	t_bool	is_set;
} t_color;

typedef struct s_colors
{
	t_color	floor;
	t_color	ceiling;
} t_colors;

typedef struct s_map
{
    char	**map;
    int		width;
    int		height;
	int		title_size; /* to do */
	int		player_x;
	int		player_y;
} t_map;

typedef struct s_cub3d
{
	t_texture textures;
	t_colors   colors;
	t_map     map;
	t_player	player;
	int			fd;
} t_cub3d;


// load_cub3d_file.c
void	load_cub3d_file(char *file, t_cub3d *cub3d);

// parse_utils.c
t_bool		check_cub_extension(char *file);
void		init_cub3d_data(t_cub3d **cub3d, int fd);
int			get_map_height(char **cube_file, size_t map_starting_i);
void		get_map_line_len(char *line, size_t *line_map_len);
t_bool 		valid_map_char(char c);
t_bool		check_if_all_walls(char *line);
void		free_2d_array(char **array);

// libft_utils.c
char    **get_lines(int fd, char **cube_file, int *line_count, t_cub3d **cub3d_data);
t_bool	space_checker(char *line);
t_bool	ft_isspace(char c);
char	**ft_split(char const *s, char c);

// load_cub3d_map.c
void		load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void		fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
void		copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i);

// load_cub_file.c
void		load_cub3d_file(char *file, t_cub3d *cub3d);
char   		 **get_lines(int fd, char **cube_file, int *line_count, t_cub3d **cub3d_data);

/* parse_errors */
void	err_exit(const char *err_msg, int close_fd, char **cube_file, char **map,
			t_cub3d **cub3d_data);
void	print_exit(const char *err_msg);

#endif // CUB3D_H