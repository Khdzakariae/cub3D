#include "../Includes/parsing.h"

t_bool space_checker(char *line)
{
	while (*line)
	{
		if (*line == '\n')
			return (true);
		if (*line != ' ')
			return (false);
		line++;
	}
	return (true);
}

t_bool ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r');
}

// static void	ft_clear(char **ptr, int current)
// {
// 	int	i;

// 	i = 0;
// 	while (i < current)
// 	{
// 		free(ptr[i]);
// 		i++;
// 	}
// 	free(ptr);
// }

// static int	checkwords(char const *s, char c)
// {
// 	size_t	i;
// 	int		words;

// 	i = 0;
// 	words = 0;
// 	while (s[i])
// 	{
// 		if (s[i] != c)
// 		{
// 			while (s[i] && s[i] != c)
// 			{
// 				i++;
// 			}
// 			words++;
// 			continue ;
// 		}
// 		i++;
// 	}
// 	return (words);
// }

// static int	fill_arrays(char const *s, char c, char **ptr, int end)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == c)
// 		{
// 			i++;
// 			continue ;
// 		}
// 		else
// 		{
// 			end = i;
// 			while (s[end] && s[end] != c)
// 				end++;
// 			ptr[j] = ft_substr(s, i, end - i);
// 			if (ptr[j] == NULL)
// 				return (ft_clear(ptr, j), 1);
// 			j++;
// 			i = end;
// 		}
// 	}
// 	return (ptr[j] = NULL, 0);
// }

void	print_tmc(t_cub3d *cub3d)
{
	// check paths
	 printf("NO: %s\n", cub3d->textures.no.path);
	printf("SO: %s\n", cub3d->textures.so.path);
	printf("WE: %s\n", cub3d->textures.we.path);
	printf("EA: %s\n", cub3d->textures.ea.path);
	// check colors
	printf("F: %x\n", cub3d->colors.floor.color);
	printf("C: %x\n", cub3d->colors.ceiling.color);
	printf("Map width: %d\n", cub3d->map.width);
    printf("Map height: %d\n", cub3d->map.height);
    // print map
    for (size_t i = 0; (int)i < cub3d->map.height; i++)
        printf("%s\n", cub3d->map.map[i]); 
}
