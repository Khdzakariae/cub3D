#ifndef _MAIN_H
#define _MAIN_H

#include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../lib/get_next_line/get_next_line.h"
# include <string.h>

#define NO_FD -1
#define TILE_SIZE 128
#define MAX_INT 2147483647

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

	double  radius;
    double  turnDirection;
    double  walkDirection;
    double  rotationAngle;
    double  moveSpeed;
    double  rotationSpeed;
} t_player;

// typedef struct s_data
// {
// 	/* mlx data */
// 	t_cub3d	*cub3d;
// }			t_data;

typedef struct s_dir_texture
{
	char	*path;
	t_bool	is_set;

	int texter_height;
    int texter_with;
    void    *img_ptr;
    char    *image_pixel_ptr;
    int     bits_per_pixel;
    int     endian;
    int     line_len;
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
	char		**cube_file;
} t_cub3d;


// load_cub3d_file.c
void	load_cub3d_file(char *file, t_cub3d *cub3d);

// parse_utils.c
t_bool		check_cub_extension(char *file);
void		init_cub3d_data(t_cub3d **cub3d, int fd);
void			get_map_dimensions(char **cube_file, size_t map_starting_i, size_t *map_height, size_t *map_width);
void		get_map_line_len(char *line, size_t *line_map_len);
t_bool 		valid_map_char(char c);
void		free_2d_array(char **array);
void    	fetch_min_map_width(char **map, size_t *min_width);
t_bool		all_cub3d_data_set(t_cub3d **cub3d);
void		get_color(char *line, int *color, t_cub3d **cub3d);
void		retrieve_color(char **split_colors, int *color, t_cub3d **cub3d);

// parse_utils2.c
char   		**get_lines(char **cube_file, int *line_count, t_cub3d **cub3d_data);
void		fetch_lines(t_cub3d **cub3d, char **lines,
				t_bool *map, int *line_count);
void		check_file_lines(char **cube_file, t_cub3d **cub3d, size_t i);
void		get_texture_path(char *line, char **path, t_cub3d **cub3d_data);
t_bool		check_if_all_walls(char *line);


// libft_utils.c
t_bool	space_checker(char *line);
t_bool	ft_isspace(char c);
char	**ft_split(char const *s, char c);
void	print_tmc(t_cub3d *cub3d);

// load_cub3d_map.c
void		load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void		fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);
void		copy_map_content(char **cube_file, t_cub3d **cub3d_data, size_t map_starting_i);

// load_cub_file.c /* DONE */
void		load_cub3d_file(char *file, t_cub3d *cub3d);
static void	file_is_valid(char **cube_file, t_cub3d **cub3d);
static void	fetch_textures(char **cube_file, t_cub3d **cub3d);
static void		fetch_colors(char **cube_file, t_cub3d **cub3d);

/* parse_errors */
void	err_exit(char *err_msg, t_cub3d **cub3d_data);
void	print_exit(const char *err_msg);
void	free_incomplete_map(t_cub3d **cub3d_data, size_t map_i, char *err_msg);
void	free_arrays_exit(char **arr1, char **arr2,
			char *err_msg, t_cub3d **cub3d);




#endif // CUB3D_H