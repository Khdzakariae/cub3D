#ifndef CUB3D_H
#define CUB3D_H

#include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "get_next_line/get_next_line.h"


typedef struct s_dir_texture
{
	char	*path;
	bool	is_set;
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
	bool	is_set;
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
} t_map;

typedef struct s_cub3d
{
	t_texture textures;
	t_colors   colors;
	t_map     map;
	int	      player_x;
	int	      player_y;
} t_cub3d;

typedef struct s_data
{
	void	*mlx;
	void	*win;
	t_cub3d	*cub3d;
}			t_data;

// load_cub3d_file.c
bool	load_cub3d_file(char *file, t_cub3d *cub3d);
void	init_cub3d_data(t_cub3d **cub3d);

// parse_utils.c
bool	check_cub_extension(char *file);
bool    space_checker(char *line);

// libft_utils.c
char    **get_lines(int fd, char **cube_file, int *line_count);
char	*ft_strdup(char *s);
char    **ft_realloc(char **cube_file, int line_count);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
bool	ft_isspace(char c);

#endif // CUB3D_H