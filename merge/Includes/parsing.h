/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zel-khad <zel-khad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 08:11:25 by achahid-          #+#    #+#             */
/*   Updated: 2024/11/30 09:21:36 by zel-khad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../lib/get_next_line/get_next_line.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define TILE_SIZE 128
# define MAX_INT 2147483647
# define PLAYER_FRAMES 34

typedef enum e_bool
{
	false,
	true
}					t_bool;

typedef	struct s_frame
{
	char	*path;
	int		frame_h;
	int		frame_w;
	void	*img_ptr;
	char	*image_pixel_ptr;
	int		bits_per_pixel;
	int		endian;
	int		line_len;
}	t_frame;

typedef struct s_player
{
	double			x;
	double			y;
	char			player_direction;
	double			radius;
	double			turndirection;
	double			walkdirection;
	double			rotationangle;
	double			movespeed;
	double			rotationspeed;
	double			sidedirection;
	t_frame frames[PLAYER_FRAMES];
	int		player_w;
	int		player_h;
}					t_player;

typedef struct s_dir_texture
{
	int				wallheight;
	char			*path;
	t_bool			is_set;
	int				texter_height;
	int				texter_with;
	void			*img_ptr;
	char			*image_pixel_ptr;
	int				bits_per_pixel;
	int				endian;
	int				line_len;
}					t_dir_texture;

typedef struct s_texture
{
	t_dir_texture	no;
	t_dir_texture	so;
	t_dir_texture	we;
	t_dir_texture	ea;
	t_dir_texture	menu;
	t_dir_texture	ciel;
}					t_texture;

typedef struct s_color
{
	int				color;
	t_bool			is_set;
}					t_color;

typedef struct s_colors
{
	t_color			floor;
	t_color			ceiling;
}					t_colors;

typedef struct s_map
{
	char			**map;
	int				width;
	int				height;
	int				title_size;
	int				player_x;
	int				player_y;
}					t_map;

typedef struct s_cub3d
{
	double			last_mouse_x;
	t_texture		textures;
	t_colors		colors;
	t_map			map;
	t_player		player;
	int				fd;
	char			**cube_file;
}					t_cub3d;

// load_cub3d_file.c
void				load_cub3d_file(char *file, t_cub3d *cub3d);

// load_cub3d_map.c
void				load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void				fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);

// load_cub3d_map_helper.c
void				map_is_valid(t_cub3d **cub3d_data);

// parse_utils.c
t_bool				check_cub_extension(char *file);
void				init_cub3d_data(t_cub3d **cub3d, int fd);
void				fetch_min_map_width(char **map, size_t *min_width);
void				get_color(char *line, int *color, t_cub3d **cub3d_data);
void				retrieve_color(char **split_colors, int *color,
						t_cub3d **cub3d_data);

// parse_utils2.c
void				get_lines(int *line_count, t_cub3d **cub3d_data);
void				fetch_lines(t_cub3d **cub3d, char **lines, t_bool *map,
						int *line_count);
void				check_file_lines(char **cube_file, t_cub3d **cub3d,
						size_t i);
void				get_texture_path(char *line, char **path,
						t_cub3d **cub3d_data);
t_bool				check_if_all_walls(char *line);

// parse_utils3.c
void				get_map_dimensions(char **cube_file, size_t map_starting_i,
						size_t *map_height, size_t *map_width);
void				get_map_line_len(char *line, size_t *line_map_len);
t_bool				valid_map_char(char c);
void				free_2d_array(char **array);
t_bool				all_cub3d_data_set(t_cub3d **cub3d);

// general_utils.c
t_bool				space_checker(char *line);
t_bool				ft_isspace(char c);
int					get_next_line_len(int fd, char **line, int *len);

// load_cub3d_map.c
void				load_cub3d_map(char **cube_file, t_cub3d **cub3d_data);
void				fill_map(char **cube_file, t_cub3d **cub3d_data, size_t *i);

/* parse_errors */
void				err_exit(char *err_msg, t_cub3d **cub3d_data);
void				print_exit(const char *err_msg);
void				free_incomplete_map(t_cub3d **cub3d_data, size_t map_i,
						char *err_msg);
void				free_arrays_exit(char **arr1, char **arr2, char *err_msg,
						t_cub3d **cub3d);

#endif /* PARSING_H */
