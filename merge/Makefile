FLAGS = -fsanitize=address -g3  

NAME = cub3D
mlx = lib/mlx/libmlx.a

INCLUDES = -I "Includes/"

SRC	 = 	$(addprefix lib/get_next_line/, get_next_line.c get_next_line_utils.c) \
		$(addprefix src/execution/, main.c game_loop.c key_events.c init_game.c raycasting.c raycasting_utils_1.c \
			test_sprite_animation.c	raycasting_utils.c render_floor_ciel.c render_textures.c render_walls.c render_walls_utils.c  cleanup.c  )  \
	    $(addprefix src/parsing/, parse_errors.c parse_utils.c load_cub3d_map.c load_cub_file.c general_utils.c \
		  					 	parse_utils2.c parse_utils3.c load_cub3d_map_helper.c )

LIBFT = lib/libft/libft.a

OBJ_DIR = .garbage_objs

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

all: $(LIBFT) $(NAME)

$(LIBFT) : $(SRC)
	@make -C lib/libft --silent
	@echo "libft DONE [✅]"

# %.o: %.c
# 	@$(CC) -c $< -o $@

# $(NAME): $(OBJ)
# 	@$(CC) $(FLAGS) $(mlx) $(OBJ) $(LIBFT) $(PRINTF) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)
# 	@echo "SO_LONG DONE [🤡]"

 %.o: %.c
	@$(CC) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

 $(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(LIBFT)   -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@echo "CUB3D DONE [🤡]"

$(OBJ_DIR)/%.o: lib/get_next_line/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR)/%.o: src/execution/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR)/%.o: src/parsing/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)


clean:
	@echo "CLEAN DONE [✅]"
	@rm -f $(OBJ) 
	@make clean -C lib/libft --silent

fclean: clean
	@echo "FCLEAN DONE [✅]"
	@rm -rf $(NAME) $(OBJ_DIR)
	@make fclean -C lib/libft --silent
	
re: fclean all
	@echo "REBUILD DONE [✅]"
