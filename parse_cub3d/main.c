#include "main.h"

int	main(int ac, char **av)
{
	t_data data;
	t_cub3d cub3d;

	if (ac != 2)
	{
		printf("Error\nInvalid number of arguments\n");
		return (EXIT_FAILURE);
	}
	if (load_cub3d_file(av[1], &cub3d) == false)
	{
		printf("Failed to load cub3d file\n");
		return (EXIT_FAILURE);
	}
	// data ready for execution.
	data.cub3d = &cub3d;
	return (0);
}
