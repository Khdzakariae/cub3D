FLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3  

NAME = cub3D

NAME_BONUS = cub3D_BONUS

mlx = lib/mlx/libmlx.a

INCLUDES = -I "mandatory/Includes/"

INCLUDES_BONUS = -I "bonus/Includes/"

HEADERS = $(addprefix mandatory/Includes/, parsing.h cub3d.h)

HEADERS_BONUS = $(addprefix bonus/Includes/, parsing_bonus.h cub3d_bonus.h)

SRC	 = 	$(addprefix lib/get_next_line/, get_next_line.c get_next_line_utils.c) \
		$(addprefix src/execution/, main.c game_loop.c key_events.c init_game.c raycasting.c raycasting_utils_1.c \
			init_game_utils.c raycasting_utils.c render_floor_ciel.c render_textures.c render_walls.c render_walls_utils.c  cleanup.c  )  \
	    $(addprefix src/parsing/, parse_errors.c parse_utils.c load_cub3d_map.c load_cub_file.c general_utils.c \
		  					 	parse_utils2.c parse_utils3.c load_cub3d_map_helper.c map_utils.c )

SRC_BONUS = $(addprefix lib/get_next_line/, get_next_line.c get_next_line_utils.c) \
		$(addprefix bonus/src/execution/, main_bonus.c game_loop_bonus.c key_events_bonus.c init_game_bonus.c raycasting_bonus.c \
			raycasting_utils_1_bonus.c init_game_utils_bonus.c render_sprite_bonus.c raycasting_utils_bonus.c render_floor_ciel_bonus.c \
			render_textures_bonus.c render_walls_bonus.c render_walls_utils_bonus.c  cleanup_bonus.c play_song_bg_bonus.c )  \
	    $(addprefix bonus/src/parsing/, parse_errors_bonus.c parse_utils_bonus.c load_cub3d_map_bonus.c load_cub_file_bonus.c general_utils_bonus.c \
		  	parse_utils2_bonus.c parse_utils3_bonus.c load_cub3d_map_helper_bonus.c map_utils_bonus.c )

LIBFT = lib/libft/libft.a

OBJ_DIR = .garbage_objs

OBJ_DIR_BONUS = bonus/.garbage_objs

OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

OBJ_BONUS = $(addprefix $(OBJ_DIR_BONUS)/, $(notdir $(SRC_BONUS:.c=.o)))

all: $(LIBFT) $(NAME)

$(LIBFT) :
	@make -C lib/libft --silent
	@echo "libft DONE [✅]"

$(NAME): $(OBJ) $(LIBFT) $(HEADERS)
	@$(CC) $(FLAGS) $(INCLUDES) $(OBJ) $(LIBFT) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@echo "CUB3D DONE [🤡]"

$(OBJ_DIR)/%.o: lib/get_next_line/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR)/%.o: mandatory/src/execution/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR)/%.o: mandatory/src/parsing/%.c | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(INCLUDES) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR_BONUS)/%.o: lib/get_next_line/%.c | $(OBJ_DIR_BONUS)
	@$(CC) $(FLAGS) $(INCLUDES_BONUS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR_BONUS)/%.o: bonus/src/execution/%.c | $(OBJ_DIR_BONUS)
	@$(CC) $(FLAGS) $(INCLUDES_BONUS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR_BONUS)/%.o: bonus/src/parsing/%.c | $(OBJ_DIR_BONUS)
	@$(CC) $(FLAGS) $(INCLUDES_BONUS) -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR_BONUS):
	@mkdir -p $(OBJ_DIR_BONUS)

bonus: $(OBJ_BONUS) $(LIBFT) $(HEADERS_BONUS)
	@$(CC) $(FLAGS) $(OBJ_BONUS) $(INCLUDES_BONUS) $(LIBFT) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME_BONUS)
	@echo "CUB3D BONUS DONE [🤡]"
	
clean:
	@echo "CLEAN DONE [✅]"
	@rm -f $(OBJ) $(OBJ_BONUS)
	@make clean -C lib/libft --silent

fclean: clean
	@echo "FCLEAN DONE [✅]"
	@rm -rf $(NAME) $(NAME_BONUS) $(OBJ_DIR) $(OBJ_DIR_BONUS)
	@make fclean -C lib/libft --silent
	
re: fclean all
	@echo "REBUILD DONE [✅]"

.PHONY: clean fclean all re
