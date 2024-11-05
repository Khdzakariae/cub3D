FLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

NAME = cub3D
mlx = mlx/libmlx.a

INCLUDES = -I "Includes/"

SRC	 =	$(addprefix utils/, utils1.c) \
		$(addprefix src/, main.c) \

LIBFT = lib/libft/libft.a

OBJ = $(SRC:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT) :$(SRC)
	@make -C lib/libft --silent
	@echo "libft DONE [✅]"

%.o: %.c
	@$(CC) -Wall -Wextra -Werror -c $< -o $@

# %.o: %.c
# 	@$(CC) -Wall -Wextra -Werror -I/usr/include -Imlx_linux -O3 -c $< -o $@

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(mlx) $(OBJ) $(LIBFT) $(PRINTF) -Lmlx -lmlx -framework OpenGL -framework AppKit -o $(NAME)
	@echo "SO_LONG DONE [🤡]"

# $(NAME): $(OBJ)
# 	@$(CC)  $(INCLUDES) $(OBJ) $(LIBFT) -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -o $(NAME)
	@echo "CUB3D DONE [🤡]"
clean:
	@echo "CLEAN DONE [✅]"
	@rm -f $(OBJ) 
	@make clean -C lib/libft --silent

fclean: clean
	@echo "FCLEAN DONE [✅]"
	@rm -f $(NAME)
	@make fclean -C lib/libft --silent
	
re: fclean all
	@echo "REBUILD DONE [✅]"


